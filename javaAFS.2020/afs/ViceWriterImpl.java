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
    ViceImpl viceimpl;
    private String name;
    private VenusCB cb;

    public ViceWriterImpl(String fileName , String modo/* añada los parámetros que requiera */, VenusCB cb)
		    throws RemoteException, FileNotFoundException  {
        file = new RandomAccessFile(AFSDir+fileName,modo);
        this.cb = cb;
        this.viceimpl = new ViceImpl();
        ViceImpl.cerrojo.bind(fileName).writeLock().lock();
        this.name=fileName;
    }
    public void write(byte [] b) throws RemoteException , IOException{
        file.write(b);
    }
    public void close() throws RemoteException, IOException {
        if(!ViceImpl.mapLock.isEmpty() && ViceImpl.mapLock.get(name)!=null){
            for (int i= 0; i< ViceImpl.mapLock.get(name).size();i++){
                if(!ViceImpl.mapLock.get(name).get(i).equals(this.cb)){
                    ViceImpl.mapLock.get(name).get(i).invalidate(name);
                    this.getVi().removeC(name,ViceImpl.mapLock.get(name).get(i));
                    i--;
                }
            }
        }
        ViceImpl.cerrojo.bind(name).writeLock().unlock();
        ViceImpl.cerrojo.unbind(name);
        file.close();
    }

    //funciones añadidas
    public void setLength (long length) throws IOException{
        file.setLength(length);
    }
    public void seek(long i) throws IOException {
        file.seek(i);
    }

    public ViceImpl getVi(){
        return this.viceimpl;
    }

    public void setVi(ViceImpl v){
        this.viceimpl=v;
    }
    
}       


