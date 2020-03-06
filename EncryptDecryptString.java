import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Base64;
import java.util.Scanner;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * Program to Encrypt/Decrypt String Using AES 128 bit Encryption Algorithm
 */
public class EncryptDecryptString {
    
    private static final String cipherTransformation    = "AES/CBC/PKCS5PADDING";
    private static final String aesEncryptionAlgorithem = "AES";
       
    public static String encrypt(String plainText) {
        String encryptedText = "";
        byte iv1[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };
       
        try {
            Cipher cipher   = Cipher.getInstance(cipherTransformation);
            byte[] key      = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };
            SecretKeySpec secretKey = new SecretKeySpec(key, aesEncryptionAlgorithem);
            IvParameterSpec ivparameterspec = new IvParameterSpec(iv1);
            cipher.init(Cipher.ENCRYPT_MODE, secretKey, ivparameterspec);
            byte[] cipherText = cipher.doFinal(plainText.getBytes("UTF-8"));
            Base64.Encoder encoder = Base64.getEncoder();
            encryptedText = encoder.encodeToString(cipherText);
            //encryptedText = new String(cipherText);

        } catch (Exception E) {
             System.err.println("Encrypt Exception : "+E.getMessage());
        }
        return encryptedText;
    }

    public static String decrypt(String encryptedText) {
        String decryptedText = "";
        byte iv1[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };
        try {
            Cipher cipher   = Cipher.getInstance(cipherTransformation);
            byte[] key      = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P' };//encryptionKey.getBytes(characterEncoding);
            SecretKeySpec secretKey = new SecretKeySpec(key, aesEncryptionAlgorithem);
            IvParameterSpec ivparameterspec = new IvParameterSpec(iv1);
            cipher.init(Cipher.DECRYPT_MODE, secretKey, ivparameterspec);
            Base64.Decoder decoder = Base64.getDecoder();
            byte[] cipherText = decoder.decode(encryptedText.getBytes("UTF8"));
            decryptedText = new String(cipher.doFinal(cipherText), "UTF-8");

        } catch (Exception E) {
            System.err.println("decrypt Exception : "+E.getMessage());
        }
        return decryptedText;
    }
    
    public static void main(String[] args) {
        //System.out.println("Enter String : ");
        System.out.println("Running.... ");
        String plainString = "&&&&&!!!!!!!!!!command123";//sc.nextLine();        
        String encyptStr   = encrypt(plainString);
        String decryptStr  = decrypt(encyptStr);
        
		Socket socketOfClient;
		try {
			socketOfClient = new Socket("127.0.0.1", 4434);
	        BufferedWriter os = new BufferedWriter(new OutputStreamWriter(socketOfClient.getOutputStream()));	
	        BufferedReader is = new BufferedReader(new InputStreamReader(socketOfClient.getInputStream()));
	        os.write(encyptStr);
	        os.flush();

        	System.out.printf("\nSent data to server....\n");
        	//os.flush();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}			  
        
        System.out.println("Plain   String  : "+plainString);
        System.out.println("Encrypt String  : "+encyptStr);
        System.out.printf("byte sent: %d\n", encyptStr.length());
        System.out.println("Decrypt String  : "+decryptStr);
        
    }   
}