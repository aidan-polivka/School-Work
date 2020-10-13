package com.example.finalapp;

import androidx.appcompat.app.AppCompatActivity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;
import org.apache.commons.io.IOUtils;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;


public class Second extends AppCompatActivity {


    ImageView imageView2;
    MediaPlayer player;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second2);

        //Establish Bitmap to iterate through
        Intent intent = getIntent();
        Uri imageUri = Uri.parse(intent.getStringExtra("image-uri"));
        if (imageUri != null) {
            imageView2 = (ImageView) findViewById(R.id.imageView2);
            imageView2.setImageURI(imageUri);
            Bitmap bitmap = ((BitmapDrawable) imageView2.getDrawable()).getBitmap();

            //Execute Async Task
            new LongRunningTask().execute(bitmap);
        }
    }

    // After Image Processing Data is Obtained, Start Audio Processing Task
    private void onBackgroundTaskDataObtained(int[][] data) {
        new AudioTask().execute(data);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////// MEDIA PLAYER
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public void playSound(View v) {
        if (player == null) {
            Uri Uristring = Uri.fromFile(file);
            player = MediaPlayer.create(this, Uristring);
            player.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    stopPlayer();
                }
            });
        }
        player.start();
    }

    public void saveSound(View v) {

    }

    public void pauseSound(View v) {
        if (player != null) {
            player.pause();
        }
    }

    public void stopSound(View v) {
        stopPlayer();
    }

    private void stopPlayer() {
        if (player != null) {
            player.release();
            player = null;
            Toast.makeText(this, "mediaPlayer released", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        stopPlayer();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////// IMAGE PROCESSING CODE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private class LongRunningTask extends AsyncTask<Bitmap, Void, int[][]> {

        private ProgressDialog progressDialog;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progressDialog = new ProgressDialog(Second.this);
            progressDialog.setMessage("Loading...");
            progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progressDialog.show();
        }

        protected int[][] doInBackground(Bitmap... params) {

            /************************* Iteration Method Through Image *****************************/

            /** This method tiles the image into 16 tiles, finds the average red, green, and blue
             pixel values of each tile, and saves them to a two dimensional integer array. **/

            //Retrieve Bitmap Parameter
            Bitmap bitmap = (Bitmap) params[0];

            //initialize all sums and our count
            int count = 0;
            int rSum = 0;
            int bSum = 0;
            int gSum = 0;

            //Compute Fragment
            int heightFragment = bitmap.getHeight() / 4;
            int widthFragment = bitmap.getWidth() / 4;

            //initialize array to return
            //16 tiles multiplied by 3 colors data[tile][r, g, or b]
            int[][] data = new int[16][3];

            //Initialize buffer sizes
            int bufferHeight = 0;
            int bufferWidth = 0;
            int index = 0;

            // Begin iteration through image
            // First Two For Loops Are for Tiling Method
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    //These Two For Loops are for Processing Image
                    for (int i = bufferHeight; i < (bufferHeight + heightFragment); i++) {
                        for (int j = bufferWidth; j < bufferWidth + widthFragment; j++) {
                            int c = bitmap.getPixel(j, i);
                            count++;
                            rSum += (Color.red(c));
                            bSum += (Color.blue(c));
                            gSum += (Color.green(c));
                        }
                    }
                    // Increase Width Fragment
                    bufferWidth += widthFragment;

                    // Calculate Tile Sums
                    rSum = rSum / count;
                    bSum = bSum / count;
                    gSum = gSum / count;

                    // Load Sums Into Array to Return
                    data[index][0] = rSum;
                    data[index][1] = bSum;
                    data[index][2] = gSum;
                    index++;

                    // Clear Sums and Count
                    count = 0;
                    rSum = 0;
                    bSum = 0;
                    gSum = 0;
                }
                // Increase Height Fragment and Restart Width
                bufferHeight += heightFragment;
                bufferWidth = 0;
            }

            return data;
        }

        protected void onPostExecute(int[][] data) {

            // Send Data to this Method to Immediately Start Audio Method
            onBackgroundTaskDataObtained(data);

            progressDialog.dismiss();

        }

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////// SOUND PROCESSING CODE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    private class AudioTask extends AsyncTask<int[][], Void, File> {

        private ProgressDialog progressDialog;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progressDialog = new ProgressDialog(Second.this);
            progressDialog.setMessage("Loading...");
            progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progressDialog.show();
        }

        protected File doInBackground(int[][]... params) {

            /**************************** Note Choosing For Chords ********************************/

            // List of File Names Stored As Strings
            String[] notes = {"a", "b", "bb", "c", "cb", "d", "e", "eb", "f", "fb", "g", "gb"};
            int numNotes = notes.length;

            // Parameters Sent to Method
            int[][] rgb = params[0];

            // Buffers and Values Assigned to Strings
            int frag = 255 / numNotes;
            int val = 0;

            // Two Dimensional Array to Assign Chords from RGB Values
            String[][] chords = new String[16][3];

            // Method to Assign Chords from RGB Values
            // R: 0, G: 1, B: 2.
            for (int h = 0; h < 3; h++) {
                // 16 Tiles makes 16 Chords in Chord Progression
                for (int i = 0; i < 16; i++) {
                    // Based on What Range the RGB Value is, Choose Note
                    for (int j = 0; j < numNotes; j++) {
                        if (rgb[i][h] > val)
                            val += frag;
                        if (rgb[i][h] <= val) {
                            chords[i][h] = notes[j];
                            j = numNotes - 1;
                        }
                    }
                    val = 0;
                }
            }

            /******************************* FILE CONCATENATION ***********************************/

            //First of Each Chord
            byte[] fileA = fileToByteNH(chords[0][0]);
            //Third of Each Chord
            byte[] fileB = fileToByteNH(chords[0][1]);
            //Fifth of Each Chord
            byte[] fileC = fileToByteNH(chords[0][2]);

            // Concatenation Method
            for ( int i = 1; i < 16; i ++ ) {
                byte[] tempA = fileToByteNH(chords[i][0]);
                fileA = concatFiles(fileA, tempA);

                byte[] tempB = fileToByteNH(chords[i][1]);
                fileB = concatFiles(fileB, tempB);

                byte[] tempC = fileToByteNH(chords[i][2]);
                fileC = concatFiles(fileC, tempC);
            }

            // Example Header to Extract Bitsize, BPS, NumChannels etc.
            byte[] tempFile = fileToByte("a");
            byte[] exampleHeader = new byte[44];
            for( int i = 0; i < 44; i ++) {
                exampleHeader[i] = tempFile[i];
            }

            // Write the Wav File to Merge the First Third and Fifth of Each Chord
            file = writeWavFile(exampleHeader, fileA, fileB, fileC);

            return file;
        }

        protected void onPostExecute( File result)  {

            progressDialog.dismiss();
        }
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////// WAV FILE PROCESSING CODE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ///********************************* File Concatenation *************************************///
    public byte[] concatFiles(byte[] file1, byte[] file2) {
        // Get File1 Length and Result length
        int file1Length = file1.length;
        int resultLength = file1Length + file2.length;

        // Instantiate Final Byte Array
        byte[] result = new byte[resultLength];

        // Store Bytes from File 1 first
        for (int i = 0; i < file1Length; i++) {
            result[i] = file1[i];
        }

        // Then Store Bytes from File 2
        int j = 0;
        for (int i = file1Length; i < resultLength; i ++) {
            result[i] = file2[j];
            j++;
        }

        return result;
    }


    ///********************************* Write The Wav File *************************************///
    //Make File Universal
    File file;
    private File writeWavFile( byte[] headerExample, byte[] file1, byte[] file2, byte[] file3 ) {

        //merge chord method
        byte[] sampleBytes = mergeFiles(file1, file2, file3);

        //write header method
        byte[] header = WriteWaveFileHeader2(headerExample, sampleBytes);

        // Create File and Input/Output Streams
        try {
            file = File.createTempFile("final", "wav", this.getCacheDir());
            try {
                FileOutputStream fos = new FileOutputStream(file);
                // Write Header and Samples to new WAV File
                fos.write(header);
                fos.write(sampleBytes);
                fos.close();
            }catch (FileNotFoundException e) {
                e.printStackTrace();
            }catch (IOException e) {
                e.printStackTrace();
            }
        }catch (IOException e) {
            e.printStackTrace();
        }

        return file;
    }


    ///*********************************** File Merging *****************************************///
    private byte[] mergeFiles(byte[] file1, byte[] file2, byte[] file3 ) {

        // Get Sample Lengths
        int sampleASize = file1.length;
        int sampleBSize = file2.length;
        int sampleCSize = file3.length;

        // Make New Sample Size the Smallest of the Sample Lengths
        // Just in Case to Avoid Array Out Of Bounds
        int sampleSize = (Math.min(Math.min(sampleASize, sampleBSize), sampleCSize))/2;

        // Initialize Short Array for Sample Manipulation
        short[] shortsA = new short[sampleASize/2];
        short[] shortsB = new short[sampleBSize/2];
        short[] shortsC = new short[sampleCSize/2];

        // Convert Byte Arrays to Short Arrays.
        ByteBuffer.wrap(file1).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shortsA);
        ByteBuffer.wrap(file2).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shortsB);
        ByteBuffer.wrap(file3).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shortsC);

        // Perform Mixing Algorithm on FileA and FileB first, then Mix FileAB with FileC
        short[] shortsAB = mixShorts(shortsA, shortsB, sampleSize);
        short[] shortsProduct = mixShorts(shortsAB, shortsC, sampleSize);

        // Convert Back to Byte Array
        byte[] results = new byte[shortsProduct.length * 2];
        ByteBuffer.wrap(results).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().put(shortsProduct);

        return results;
    }


    ///******************************* Algorithm to Merge Files *********************************///
    public short[] mixShorts(short[] samplesA, short[] samplesB, int size) {

        // Convert Shorts To Double
        double[] doubleA = new double[size];
        double[] doubleB = new double[size];

        // Initialize Max Values to Zero
        double maxA = 0;
        double maxB = 0;
        for ( int i = 0; i < size; i++ ) {

            // Convert Shorts to Doubles, Divide by Max 16 bit Byte Integer Value
            doubleA[i] = (double)samplesA[i]/32767;
            // Compute Max of Double Array A
            double A = Math.abs(doubleA[i]);
            if(A > maxA)
                maxA = A;

            // Convert Shorts to Doubles, Divide by Max 18 bit Byte Integer Value
            doubleB[i] = (double)samplesB[i]/32767;
            // Compute Max of Double Array B
            double B = Math.abs(doubleB[i]);
            if(B > maxB)
                maxB = B;
        }

        // Create Scaling Factor for Mixing
        double x = 1 / (maxA + maxB);

        // Initialize Final Short Array
        short[] result = new short[size];

        // Mix Doubles and Convert to Shorts
        for (int i = 0; i < size; i++){
            result[i] = (short) (x * (doubleA[i] + doubleB[i]) * 32767);
        }

        return result;
    }


    ///****************************** Write Wav File Header *************************************///
    private byte[] WriteWaveFileHeader2( byte[] headerExample, byte[] file) {

        // The Header of a Wav File is 44 Bytes Long
        int headerLength = 44;

        // Retrieve Length of Sound Data
        long sampleLength = file.length;
        System.out.println(sampleLength);

        // Retrieve Size of File Described by Header
        long dataSize = sampleLength + 36;

        // Initialize header Byte Array
        byte[] header = new byte[headerLength];

        // Use the Same Header as One of The Note Files
        // Update File Size and Sample Size
        for( int i = 0; i < headerLength; i++ ){
            if( i == 4) {
                header[4] = (byte) (dataSize & 0xff);
                header[5] = (byte) ((dataSize >> 8) & 0xff);
                header[6] = (byte) ((dataSize >> 16) & 0xff);
                header[7] = (byte) ((dataSize >> 24) & 0xff);
                i += 3;
            }else if (i == 40) {
                header[40] = (byte) (sampleLength & 0xff);
                header[41] = (byte) ((sampleLength >> 8) & 0xff);
                header[42] = (byte) ((sampleLength >> 16) & 0xff);
                header[43] = (byte) ((sampleLength >> 24) & 0xff);
                i += 3;
            } else {
                header[i] = headerExample[i];
            }
        }

        return header;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////// Other Helper Methods
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ///********************************* Get Resource ID ****************************************///
    public static int getResId(String resName, Class<?> c) {

        try {
            Field idField = c.getDeclaredField(resName);
            return idField.getInt(idField);
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }


    ///**************************** Convert File To Byte Array **********************************///
    public byte[] fileToByte(String fileName) {
        byte[] result;
        try {
            result = IOUtils.toByteArray(this.getResources().openRawResource(
                    getResId(fileName, R.raw.class)));
            return result;
        } catch (IOException e) {
            e.printStackTrace();
            byte[] failed = {(byte) 0 & 0xff};
            return failed;
        }
    }


    ///********************* Convert File To Byte Array and Extract Header **********************///
    public byte[] fileToByteNH(String fileName) {
        byte[] temp;
        try {
            temp = IOUtils.toByteArray(this.getResources().openRawResource(
                    getResId(fileName, R.raw.class)));
            int length = temp.length - 44;
            byte[] result = new byte[length];

            // Extract Header (First 44 Bytes of WAV File)
            int j = 0;
            for(int i = 44; i < length; i++){
                result[j] = temp[i];
                j++;
            }
            return result;
        } catch (IOException e) {
            e.printStackTrace();
            byte[] failed = {(byte) 0 & 0xff};
            return failed;
        }
    }
}

/**

 _______________________¶¶¶¶¶¶¶¶___________________
 _______________________¶¶¶¶¶¶¶¶¶¶¶¶¶______________
 ___________¶¶__________¶¶¶¶¶____¶¶¶¶______________
 _________¶¶¶¶¶_________¶¶¶¶¶¶¶¶¶¶¶¶¶______________
 ________¶¶¶¶¶¶¶________¶¶¶___¶¶¶¶¶¶¶____¶¶¶_______
 _______¶¶¶__¶¶¶________¶¶¶________¶¶¶___¶¶¶_______
 _______¶¶___¶¶¶_________¶¶________¶¶¶____¶¶¶______
 ______¶¶¶__¶¶¶¶_________¶¶_________¶¶_____¶¶¶_____
 ______¶¶¶_¶¶¶¶______¶¶__¶¶¶________¶¶¶_____¶¶¶____
 _______¶¶¶¶¶¶_____¶¶¶¶¶¶¶¶¶________¶¶¶______¶¶¶___
 ______¶¶¶¶¶¶_____¶¶¶¶¶¶¶¶¶¶__¶¶¶¶¶¶¶¶¶_______¶¶¶__
 ____¶¶¶¶¶¶_______¶¶¶¶¶¶¶¶¶¶_¶¶¶¶¶¶¶¶¶¶________¶¶¶_
 ___¶¶¶¶¶¶¶________¶¶¶¶¶¶¶___¶¶¶¶¶¶¶¶¶¶_________¶¶_
 __¶¶¶¶__¶¶¶¶¶¶¶¶_____¶¶______¶¶¶¶¶¶¶¶______¶¶¶¶¶¶¶
 _¶¶¶¶__¶¶¶¶¶¶¶¶¶¶¶___¶¶¶¶¶_____¶¶¶________¶¶¶¶¶¶¶¶
 ¶¶¶¶__¶¶¶¶¶¶¶¶¶¶¶¶¶__¶¶¶¶¶¶¶¶____________¶¶¶¶¶¶¶¶¶
 ¶¶¶¶_¶¶¶¶¶¶¶___¶¶¶¶___¶¶¶¶¶¶¶¶¶¶_________¶¶¶¶¶¶¶¶¶
 ¶¶¶__¶¶¶__¶¶____¶¶¶____¶¶¶_¶¶¶¶¶¶¶________¶¶¶¶¶¶¶¶
 ¶¶¶¶__¶¶__¶¶¶___¶¶¶____¶¶¶____¶¶¶¶¶__________¶¶___
 _¶¶¶¶_____¶¶¶__¶¶¶¶_____¶¶¶_____¶¶¶¶______________
 _¶¶¶¶¶¶¶___¶¶¶¶¶¶¶_______¶¶¶_____¶¶¶______________
 ___¶¶¶¶¶¶¶¶¶¶¶¶¶_________¶¶¶______¶¶______________
 _______¶¶¶¶¶¶¶____________¶¶¶_____¶_______________
 ____________¶¶¶____________¶¶¶____________________
 ____¶¶¶¶¶¶__¶¶¶________¶¶¶¶¶¶¶____________________
 ____¶¶¶¶¶¶¶_¶¶¶______¶¶¶¶¶¶¶¶¶¶___________________
 ____¶¶¶¶¶¶_¶¶¶______¶¶¶¶¶¶¶¶¶¶¶¶__________________
 _____¶¶¶¶¶¶¶¶¶______¶¶¶¶¶¶¶¶¶¶¶___________________
 _______¶¶¶¶¶_________¶¶¶¶¶¶¶¶¶____________________
 ______________________¶¶¶¶¶¶______________________

 **/