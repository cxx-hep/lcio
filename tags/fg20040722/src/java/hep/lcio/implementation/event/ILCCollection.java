package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.LCParameters;

import hep.lcio.event.LCCollection;

import hep.lcio.exceptions.ReadOnlyException;

import java.util.ArrayList;


/**
 * A default implementation of LCCollection
 * @author Tony Johnson
 * @version $Id: ILCCollection.java,v 1.8 2004-07-14 15:50:44 gaede Exp $
 */
public class ILCCollection extends ArrayList implements LCCollection
{
   private String type;
   private int flag;
   protected ILCParameters parameters = new ILCParameters() ;

   public ILCCollection(String type)
   {
      this.type = type;
   }

   public ILCCollection(String type, int flag, int size)
   {
      super(size);
      this.flag = flag;
      this.type = type;
   }

   public LCObject getElementAt(int index)
   {
      return (LCObject) get(index);
   }

   public void setFlag(int flag)
   {
      this.flag = flag;
   }

   public int getFlag()
   {
      return flag;
   }

   public int getNumberOfElements()
   {
      return size();
   }

   public String getTypeName()
   {
      return type;
   }

   public void add(LCObject object)
   {
      checkAccess();
      super.add(object);
   }

   public void addElement(LCObject obj) throws ReadOnlyException
   {
      checkAccess();
      super.add(obj);
   }

   public void removeElementAt(int i) throws ReadOnlyException
   {
      checkAccess();
      super.remove(i);
   }

   protected void checkAccess()
   {
      
   }
   public LCParameters getParameters() {
	   return parameters;
   }
   
   public void setParameters(LCParameters p) {
	   if(p!=null)
	     parameters = (ILCParameters) p ; 
   }


}