package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;

import java.util.BitSet;
import java.util.List;

/**
 *
 * @author Tony Johnson
 */
public class ICluster extends ILCObject implements Cluster
{
   protected float energy;
   protected float phi;
   protected float theta;
   //protected int type;
   protected BitSet type ;
   protected float[] directionError;
   protected float[] hitContributions;
   protected float[] particleType;
   protected float[] position;
   protected float[] shape;
   protected float[] positionError;
   protected float[] subdetectorEnergies ;
   
   public List getCalorimeterHits()
   {
      return null;
   }
   
   public List getClusters()
   {
      return null;
   }
   
   public float[] getDirectionError()
   {
      return directionError;
   }
   
   public float getEnergy()
   {
      return energy;
   }
   
   public float[] getHitContributions()
   {
      return hitContributions;
   }
   
   public float[] getParticleType()
   {
      return particleType;
   }
   
   public float getPhi()
   {
      return phi;
   }
   
   public float[] getPosition()
   {
      return position;
   }
   
   public float[] getPositionError()
   {
      return positionError;
   }
   
   public float[] getShape()
   {
      return shape;
   }
   
   public float getTheta()
   {
      return theta;
   }
   
   public int getType() {
	   int intType = 0;
	   for (int i = 0; i < 32; i++) {
		   if (type.get(i)) {
			   intType |= 1 << i;
		   }
	   }
	   return intType;
   }

//   public int getType()
//   {
//      return type;
//   }
   
   public boolean testType(int bitIndex) {

	   return type.get(bitIndex);
   }


  public void setCalorimeterHits(List calorimeterHits)
   {
      // FIXME:
   }
   
   public void setClusters(List clusters)
   {
      // FIXME:
   }
   
   public void setDirectionError(float[] directionError)
   {
      this.directionError = directionError;
   }
   
   public void setEnergy(float energy)
   {
      this.energy = energy;
   }
   
   public void setHitContributions(float[] hitContributions)
   {
      this.hitContributions = hitContributions;
   }
   
   public void setParticleType(float[] particleType)
   {
      this.particleType = particleType;
   }
   
   public void setPhi(float phi)
   {
      this.phi = phi;
   }
   
   public void setPosition(float[] position)
   {
      this.position = position;
   }
   
   public void setPositionError(float[] positionError)
   {
      this.positionError = positionError;
   }
   
   public void setShape(float[] shape)
   {
      this.shape = shape;
   }
   
   public void setTheta(float theta)
   {
      this.theta = theta;
   }
   
//   public void setType(int type)
//   {
//      this.type = type;
//   }
   protected void setType(int typeWord) {

	   for (int i = 0; i < 32; i++) {
		   if ((typeWord & (1 << i)) > 1)
			   type.set(i);
		   else
			   type.clear(i);
	   }
   }
   public void setTypeBit(int bit) {
	   type.set(bit) ;
   }
   
   public float[] getSubdetectorEnergies() {
	 return subdetectorEnergies;
   }

   public void setSubdetectorEnergies(float[] fs) {
	 subdetectorEnergies = fs;
   }

}