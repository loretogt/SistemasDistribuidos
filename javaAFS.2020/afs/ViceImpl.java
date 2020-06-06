// Implementación de la interfaz de servidor que define los métodos remotos
// para iniciar la carga y descarga de ficheros
package afs;

import java.io.FileNotFoundException;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;

public class ViceImpl extends UnicastRemoteObject implements Vice {

      public static LockManager cerrojo;
      public static HashMap<String, List<VenusCB>> mapLock = new HashMap<String, List<VenusCB>>();
     

      public ViceImpl() throws RemoteException {
            ViceImpl.cerrojo =  new LockManager();
            
      }
      public ViceReader download(String fileName, String modo, VenusCB cb/* añada los parámetros que requiera */)
                  throws RemoteException, FileNotFoundException {
            ViceReader viceReaderI = new ViceReaderImpl(fileName, modo, cb);
            return viceReaderI;
      }

      public ViceWriter upload(String fileName, String modo, VenusCB cb/* añada los parámetros que requiera */)
                  throws RemoteException, FileNotFoundException {
            ViceWriter viceWriterU = new ViceWriterImpl(fileName, modo, cb);
            return viceWriterU;
      }

      public synchronized HashMap<String,List<VenusCB>> addC(String fichero, VenusCB cb) {
            if(ViceImpl.mapLock.isEmpty() || ViceImpl.mapLock.get(fichero) == null) {
                  List<VenusCB> lista = new LinkedList<VenusCB>();
                  lista.add(cb);
                  ViceImpl.mapLock.put(fichero, lista);
            }
            else{
                  ViceImpl.mapLock.get(fichero).add(cb);
            }
            return ViceImpl.mapLock;
      }

      public synchronized void removeC(String fichero, VenusCB cb) {
            ViceImpl.mapLock.get(fichero).remove(cb);
      }
}
