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
    RandomAccessFile file;
    ViceImpl vi;
    private String name;
    
    public ViceReaderImpl(final String fileName , String modo/* añada los parámetros que requiera */, VenusCB cb)
		    throws RemoteException, FileNotFoundException {
            file = new RandomAccessFile(AFSDir+fileName,modo);
            this.vi = new ViceImpl();
            ViceImpl.cerrojo.bind(fileName).readLock().lock();
            this.getVi().addC(fileName, cb);
            this.name=fileName;
    }
    public byte[] read(final int tam) throws RemoteException, IOException {
        byte[] bytesLeidos = new byte [tam];
        int num = file.read(bytesLeidos);
        if(num<0){
            return null;
        }
        if(num<tam){ //si lo que has leido es menor que el bloque, hay que copiarlo byte a byte para no escribir de más
            byte [] res = new byte [num];
            for (int i=0; i<num; i++){
                res[i]=bytesLeidos[i];
            }
            return res;
        }
        return bytesLeidos; //cuando si que es el tamaño del bloque puedes hacerlo sin más
    }
    public void close() throws RemoteException, IOException {
        ViceImpl.cerrojo.bind(name).readLock().unlock();
        ViceImpl.cerrojo.unbind(name);
        file.close();
    }

    public ViceImpl getVi(){
        return this.vi;
    }

    public void setVi(ViceImpl v){
        this.vi=v;
    }
}       

