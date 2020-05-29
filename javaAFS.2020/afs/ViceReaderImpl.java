// Implementación de la interfaz de servidor que define los métodos remotos
// para completar la descarga de un fichero
package afs;

import java.io.*;
import java.rmi.*;
import java.rmi.server.*;
import java.io.IOException;
import java.io.RandomAccessFile;


public class ViceReaderImpl extends UnicastRemoteObject implements ViceReader {
    private static final String AFSDir = "AFSDir/";
    File f;
    RandomAccessFile file;
    
    public ViceReaderImpl(final String fileName , String modo/* añada los parámetros que requiera */)
		    throws RemoteException, FileNotFoundException {
            f = new File (AFSDir+fileName);
            //file = new RandomAccessFile(f, "rw");
            file = new RandomAccessFile(AFSDir+fileName,modo);
    }
    public byte[] read(final int tam) throws RemoteException, IOException {
        byte[] bytesLeidos = new byte [tam];
        for(int i = 0; i<tam ;i++){
            bytesLeidos[i] = file.readByte();
        }
        return bytesLeidos;
    }
    public void close() throws RemoteException, IOException {
        file.close();
    }
}       

