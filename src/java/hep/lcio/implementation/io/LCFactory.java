package hep.lcio.implementation.io;

import hep.lcio.implementation.sio.SIOFactory;

import hep.lcio.io.ILCFactory;


/**
 *
 * @author Tony Johnson
 * @version $Id: LCFactory.java,v 1.2.2.1 2004-04-06 11:26:02 gaede Exp $
 */
public abstract class LCFactory implements ILCFactory
{
   public static ILCFactory getInstance()
   {
      return new SIOFactory();
   }
}
