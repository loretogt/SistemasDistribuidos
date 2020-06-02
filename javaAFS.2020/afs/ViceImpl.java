// Implementación de la interfaz de servidor que define los métodos remotos
// para iniciar la carga y descarga de ficheros
package afs;
import java.io.FileNotFoundException;
import java.rmi.*;
import java.rmi.server.*;


public class ViceImpl extends UnicastRemoteObject implements Vice {
    public ViceImpl() throws RemoteException {
    }
    
    public ViceReader download(String fileName, String modo /* añada los parámetros que requiera */)
          throws RemoteException,FileNotFoundException {
          ViceReader viceReaderI= new ViceReaderImpl(fileName, modo);
          return viceReaderI;
       
    }
    public ViceWriter upload(String fileName, String modo /* añada los parámetros que requiera */)
          throws RemoteException ,FileNotFoundException {
            ViceWriter viceWriterU= new ViceWriterImpl(fileName, modo);
            return viceWriterU;
    }
}
