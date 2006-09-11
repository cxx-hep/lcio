package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcio.event.Vertex;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.implementation.event.IVertex;

import java.util.TreeMap;

import java.io.IOException;

/**
* @author engels
*/

class SIOVertex extends IVertex{
	public SIOVertex(SIOInputStream in, SIOEvent owner, String[] keys, int major, int minor) throws IOException{
		this.primary = in.readInt();

		//dbg
		//int t=in.readInt();
		//this.type = keys[t];
    	//System.out.println("Reading... (int)["+t+"] (string)["+type+"]");
    	
    	this.type = keys[in.readInt()];
		this.chi2 = in.readFloat();
		this.probability = in.readFloat();
		for (int i=0; i<3; i++) this.position[i] = in.readFloat();
		for (int i=0; i<6; i++) this.covMatrix[i] = in.readFloat();
		int n = in.readInt();
	    this.parameters = new float[n];
	    for (int i=0; i<n; i++) this.parameters[i] = in.readInt();
	    this.aRecP = (ReconstructedParticle) in.readPntr().getObject();
		in.readPTag(this);
	}
	static void write(Vertex v, SIOOutputStream out, TreeMap keys) throws IOException{
		if (v instanceof SIOVertex){
			((SIOVertex) v).write(out, keys);
		}
	    else{
	    	out.writeInt(v.isPrimary()?1:0);
	    	int at=(Integer)keys.get(v.getAlgorithmType());
	    	out.writeInt(at);
	    	//int[] at=(int[])keys.get(v.getAlgorithmType());
	    	//out.writeInt(at[0]);
	    	
	    	//dbg
	    	//System.out.println("Writing... (string)["+v.getAlgorithmType()+"] (int)["+at+"]");

	    	out.writeFloat(v.getChi2());
	    	out.writeFloat(v.getProbability());
	    	float[] pos = v.getPosition();
	    	for (int i=0; i<3; i++) out.writeFloat(pos[i]);
	    	float[] matrix = v.getCovMatrix();
	    	for (int i=0; i<6; i++) out.writeFloat(matrix[i]);
	    	float[] pars = v.getParameters();
	    	int n = (pars == null ? 0 : pars.length);
	    	out.writeInt(n);
	    	for (int i=0; i<n; i++) out.writeFloat(pars[i]);
	    	out.writePntr(v.getAssociatedParticle());
	    	out.writePTag(v);
	      }
	   }
	private void write(SIOOutputStream out, TreeMap keys) throws IOException{
		out.writeInt(primary);
		int at=(Integer)keys.get(type);
		out.writeInt(at);
		out.writeFloat(chi2);
		out.writeFloat(probability);
		for (int i=0; i<3; i++) out.writeFloat(position[i]);
		for (int i=0; i<6; i++) out.writeFloat(covMatrix[i]);
		int n = parameters == null ? 0 : parameters.length;
		out.writeInt(n);
		for (int i=0; i<n; i++) out.writeFloat(parameters[i]);
		out.writePntr(aRecP);
		out.writePTag(this);
	}
}
