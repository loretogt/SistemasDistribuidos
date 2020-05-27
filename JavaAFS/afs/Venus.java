// Clase de cliente que inicia la interacción con el servicio de
// ficheros remotos
package afs;

import java.net.MalformedURLException;
import java.rmi.*;

public class Venus {
    private String host;
    private int port;
    private int blocksize;
    private Vice vi;

    public Venus() throws RemoteException, MalformedURLException, NotBoundException {
        this.host= System.getenv().get("REGISTRY_HOST");
        this.port= Integer.parseInt(System.getenv().get("REGISTRY_PORT"));
        this.blocksize= Integer.parseInt(System.getenv().get("BLOCKSIZE"));
        this.vi=(Vice)Naming.lookup("//" + this.host + ":" + this.port + "/AFS");
    }

    public String getHost(){
        return this.host;
    }

    public int getPort(){
        return this.port;
    }

    public  int getBlocksize(){
        return this.blocksize;
    }

    public Vice getVi(){
        return this.vi;
    }

}

