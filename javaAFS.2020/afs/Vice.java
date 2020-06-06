// Interfaz de servidor que define los métodos remotos para iniciar
// la carga y descarga de ficheros
package afs;
import java.io.FileNotFoundException;
import java.rmi.*;

public interface Vice extends Remote {
    public ViceReader download(String fileName,String modo, VenusCB cb /* añada los parámetros que requiera */)
          throws RemoteException, FileNotFoundException;
    public ViceWriter upload(String fileName,String modo, VenusCB cb /* añada los parámetros que requiera */)
          throws RemoteException, FileNotFoundException;

    /* añada los métodos remotos que requiera */
   // public void add (String fichero, VenusCB cb);

  //  public void remove(String fichero, VenusCB cb);
}
       

