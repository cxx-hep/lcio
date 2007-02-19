package hep.lcio.util;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * Command line handling for the count command.
 * 
 * @author jeremym
 * @version $Id: HeaderCountCommandHandler.java,v 1.3 2006-12-07 00:47:32 jeremy Exp $
 */
public class HeaderCountCommandHandler extends CommandHandler
{
	boolean printRun = false;
	boolean printEvent = false;
	boolean printTot = false;
	List files = new ArrayList();
	
	HeaderCountCommandHandler()
	{
		super("count","Count number of run and event headers.");
		
		options = createHeaderCountOptions();
	}
	
	/**
	 * Create the options for the count command. 
	 * @return Options object with HeaderCount options.
	 */
	private static Options createHeaderCountOptions()
	{
		Options options = new Options();
		
		Option opt = new Option("h", false, "Print count usage.");
		options.addOption(opt);
		
		opt = new Option("r", false, "Print number of run headers.");
		options.addOption(opt);
		
		opt = new Option("e", false, "Print number of event headers.");
		options.addOption(opt);
		
		opt = new Option("t", false, "Print totals of all files processed.");
		options.addOption(opt);
		
		opt = new Option("f", true, "Add an input LCIO file.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}

	/**
	 * Parse the command line for the count command.
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);
		
		if ( cl.hasOption("h") )
		{
			printUsage(true);
		}
		
		if (cl.hasOption("r"))
		{
			printRun = true;
		}
		
		if (cl.hasOption("e"))
		{
			printEvent = true;
		}
		
		if (cl.hasOption("t")) {
			printTot = true;
		}
		
		if (!printRun && !printEvent && !printTot)
		{
			printEvent = true;
		}
		
		if (cl.hasOption("f"))
		{
			String[] f = cl.getOptionValues("f");
			for (int i=0; i<f.length; i++)
			{
				files.add(new File(f[i]));
			}
		}
		// Interpret extras as input files.
		else if (cl.getArgList() != null)
		{
			for (Object o : cl.getArgList())
			{
				String s = (String)o;
				files.add(new File(s));
			}			
		}
		else {
			printUsage(true);
		}
	}

	/**
	 * Execute the count command with current arguments.
	 */
	public void execute() throws Exception
	{
		HeaderCount.countHeaders(System.out, files, printEvent, printRun, printTot);
	}
}