// Clase de cliente que define la interfaz a las aplicaciones.
// Proporciona la misma API que RandomAccessFile.
package afs;

import java.rmi.*;
import java.io.*;
public class VenusFile {
    public static final String cacheDir = "Cache/";
    //variables para los ficheros
    ViceReader vReader;
    ViceWriter vWriter;
    RandomAccessFile fich;
    // variables para cuando escribes o cambia el tamaño
    boolean escrito;
    long tamViejo = 0;
    // variables para guardar la info
    Venus venus;
    String fileName;
    String modo;

    public VenusFile(final Venus venus, final String fileName, final String modo)
            throws RemoteException, IOException, FileNotFoundException {
        final File local = new File(cacheDir + fileName);
        this.venus = venus;
        this.fileName = fileName;
        this.modo = modo;
        try {
            if (!local.exists()) {
                fich = new RandomAccessFile(cacheDir + fileName, modo);
                vReader = venus.getVi().download(fileName, modo, venus.getCb());
                byte[] array;
                while ((array = vReader.read(venus.getBlocksize())) != null) {
                    fich.write(array);
                }
                tamViejo = fich.length();
                fich.seek(0);
                vReader.close();
            } else { // si si que está en cache
                fich = new RandomAccessFile(cacheDir + fileName, modo);
                tamViejo = fich.length();
            }
        } catch (FileNotFoundException e) {
            vReader = venus.getVi().download(fileName, modo, venus.getCb()); // si no exite al llamar al download saltara en FileNotFoundException
            fich = new RandomAccessFile(cacheDir + fileName, "rw"); // se pone en rw para que no de fallo otra vez
            byte[] array;
            while ((array = vReader.read(venus.getBlocksize())) != null) {
                fich.write(array);
            }
            tamViejo = fich.length();
            //para abrilo con su verdadero permiso
            fich.close();
            fich = new RandomAccessFile(cacheDir + fileName, modo);
            vReader.close();
        }
    }

    public int read(final byte[] b) throws RemoteException, IOException {
        return fich.read(b);
    }

    public void write(final byte[] b) throws RemoteException, IOException {
        fich.write(b);
        escrito = true;
    }

    public void seek(final long p) throws RemoteException, IOException {
        fich.seek(p);
    }

    public void setLength(final long l) throws RemoteException, IOException {
        fich.setLength(l);
    }

    public void close() throws RemoteException, IOException {
        if (escrito == true ) { // si se ha escrito en el fichero que esta en caché
            fich.seek(0); // para poder copiarlo hay que hacerlo desde el principio
            vWriter = this.venus.getVi().upload(this.fileName, modo, this.venus.getCb());
            vWriter.seek(0);
            byte[] array = new byte [this.venus.getBlocksize()];
            int num;
            while ((num =this.fich.read(array)) > 0) {
                if (num<this.venus.getBlocksize()){ //sino se hace esto no funciona (misma logica que en el read)
                    byte [] res = new byte [num];
                    for (int i=0; i<num; i++){
                        res[i]=array[i];
                    }
                    vWriter.write(res);
                }
                else{ //si si que ocupa un bloque puedo escribirlo entero
                    vWriter.write(array);
                }
            } 
            vWriter.close();
        }  
        if (fich.length() != tamViejo) { // si los tamaños son distintos hay que actualizarlos
            vWriter = this.venus.getVi().upload(this.fileName, modo, this.venus.getCb());
            vWriter.setLength(fich.length());
            vWriter.close();
        } 
        fich.close();
    }
}
