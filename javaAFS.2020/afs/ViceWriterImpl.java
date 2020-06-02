// Implementación de la interfaz de servidor que define los métodos remotos
// para completar la carga de un fichero
package afs;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.rmi.*;
import java.rmi.server.*;

public class ViceWriterImpl extends UnicastRemoteObject implements ViceWriter {
    private static final String AFSDir = "AFSDir/";
    RandomAccessFile file;

    public ViceWriterImpl(String fileName , String modo/* añada los parámetros que requiera */)
		    throws RemoteException, FileNotFoundException  {
        file = new RandomAccessFile(AFSDir+fileName,modo);
    }
    public void write(byte [] b) throws RemoteException , IOException{
        file.write(b);
    }
    public void close() throws RemoteException, IOException {
        file.close();
    }
}       

