package hep.lcio.implementation.event;

import hep.lcio.event.Cluster;
import hep.lcio.event.ParticleID;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Track;
import java.util.ArrayList;
import java.util.Collections;

import java.util.List;


/**Implementation of ReconstructedParticle
 * @author gaede
 *
 */
public class IReconstructedParticle extends ILCObject implements ReconstructedParticle
{
   protected ParticleID particleIDUsed;
   protected List clusters;
   protected List particleIDs;
   protected List particles;
   protected List tracks;
   protected float[] covMatrix = new float[10];
   protected float[] momentum = new float[3];
   protected float[] referencePoint = new float[3];
   protected float charge;
   protected float energy;
   protected float mass;
   protected float goodnessOfPID;
   protected int type;
   
   public float getGoodnessOfPID()
   {
      return goodnessOfPID;
   }
   
   public void setGoodnessOfPID(float goodnessOfPID)
   {
      checkAccess();
      this.goodnessOfPID = goodnessOfPID;
   }
   
   public void setCharge(float charge)
   {
      checkAccess();
      this.charge = charge;
   }
   
   public float getCharge()
   {
      return charge;
   }
   
   public void setClusters(List clusters)
   {
      checkAccess();
      this.clusters = clusters;
   }
   
   public List getClusters()
   {
      return clusters == null ? Collections.EMPTY_LIST : clusters;
   }
   
   public boolean isCompound()
   {
      return particles != null && !particles.isEmpty();
   }
   
   public void setCovMatrix(float[] cov)
   {
      if (cov.length != 10) throw new IllegalArgumentException();
      checkAccess();
      this.covMatrix = cov;
   }
   
   public float[] getCovMatrix()
   {
      return covMatrix;
   }
   
   public void setEnergy(float e)
   {
      checkAccess();
      this.energy = e;
   }
   
   public float getEnergy()
   {
      return energy;
   }
   
   public void setMass(float f)
   {
      mass = f;
   }
   
   public float getMass()
   {
      return mass;
   }
   
   public void setMomentum(float[] fs)
   {
      if (fs.length != 3) throw new IllegalArgumentException();
      checkAccess();
      momentum = fs;
   }
   
   public float[] getMomentum()
   {
      return momentum;
   }
   
   public ParticleID getParticleIDUsed()
   {
      return particleIDUsed;
   }
   public void setParticleIDs(List ids)
   {
      checkAccess();
      this.particleIDs = ids;
   }
   
   public List getParticleIDs()
   {
      return particleIDs == null ? Collections.EMPTY_LIST : particleIDs;
   }
   public void setParticles(List particles)
   {
      checkAccess();
      this.particles = particles;
   }   
   public List getParticles()
   {
      return particles == null ? Collections.EMPTY_LIST : particles;
   }
   
   public void setReferencePoint(float[] fs)
   {
      if (fs.length != 3) throw new IllegalArgumentException();
      checkAccess();
      referencePoint = fs;
   }
   
   public float[] getReferencePoint()
   {
      return referencePoint;
   }
   
   public void setTracks(List tracks)
   {
      checkAccess();
      this.tracks = tracks;
   }
   public List getTracks()
   {
      return tracks == null ? Collections.EMPTY_LIST : tracks;
   }
   
   public void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
   
   public int getType()
   {
      return type;
   }
   
   public void addCluster(Cluster cluster)
   {
      if (clusters == null) clusters = new ArrayList();
      clusters.add(cluster);
   }
   public void addTrack(Track track)
   {
      if (tracks == null) tracks = new ArrayList();
      tracks.add(track);
   }  
   public void addParticle(ReconstructedParticle particle)
   {
      if (particles == null) particles = new ArrayList();
      particles.add(particle);
   }
   
   public void addParticleID(ParticleID pid)
   {
      if (particleIDs ==  null) particles = new ArrayList();
      particleIDs.add(pid);
   }
   
}
