// Clase de cliente que define la interfaz a las aplicaciones.
// Proporciona la misma API que RandomAccessFile.
package afs;

import java.rmi.*; 
import java.io.*; 

public class VenusFile {
    public static final String cacheDir = "Cache/";
    private static final String AFSDir = "AFSDir/";
    int excep=1;
    ViceReader vReader;
    RandomAccessFile fich;
    boolean escrito;
    public VenusFile(final Venus venus, final String fileName, final String modo) throws RemoteException, IOException, FileNotFoundException {
        final File local = new File(cacheDir+fileName);
        try{
            if(!local.exists()){
                fich = new RandomAccessFile(cacheDir+fileName, modo); 
                vReader = venus.getVi().download(fileName, modo);
                byte [] array;
                while ((array= vReader.read(venus.getBlocksize()))!=null ){
                    fich.write(array);
                }
                fich.seek(0);
                vReader.close();
              }  
            else{ //si si que está en cache
                fich = new RandomAccessFile(cacheDir+fileName, modo); 
            } 
        }catch (FileNotFoundException e){
            //si no exite al llamar al download saltara en FileNotFoundException
            vReader = venus.getVi().download(fileName, modo);
            //se pone en rw para que no de fallo otra vez
            fich = new RandomAccessFile(cacheDir+fileName, "rw"); 
            byte [] array;
             while ((array= vReader.read(venus.getBlocksize()))!=null ){
                 fich.write(array);
            }
            fich.close();
            fich = new RandomAccessFile(cacheDir+fileName, modo);     
            vReader.close();   
          } 
    }

    public int read(final byte[] b) throws RemoteException, IOException {
        return fich.read(b);
    }
    public void write(final byte[] b) throws RemoteException, IOException {
        escrito=true;
        fich.write(b);
    }
    public void seek(final long p) throws RemoteException, IOException {
        fich.seek(p);
    }
    public void setLength(final long l) throws RemoteException, IOException {
        fich.setLength(l);
    }
    public void close() throws RemoteException, IOException {
        fich.close();
    }
   
}

