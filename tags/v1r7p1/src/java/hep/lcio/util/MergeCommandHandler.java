package hep.lcio.util;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.Parser;
import org.apache.commons.cli.PosixParser;

/**
 * This is the CommandHandler for the merge utility.
 * 
 * It parses 'lcio merge' command-line options and 
 * passes the results to a method from MergeUtil.
 * 
 * @author jeremym
 * @version $Id: MergeCommandHandler.java,v 1.5 2006-04-28 23:38:48 jeremy Exp $
 */
public class MergeCommandHandler extends CommandHandler
{
	Parser parser = new PosixParser();
	File outfile;
	File[] infiles;
	int maxevents = Integer.MAX_VALUE;
	List mergeFiles;
	float def_dt = 0;
	float def_startt = 0;
	int def_ntoread = 1;
	
	/** 
	 * MergeCommandHandler ctor. 
	 */
	MergeCommandHandler()
	{
		// Call CommandHandler ctor.
		super("merge", "Merge LCIO events together.");
		
		// Setup merge command options.
		options = createMergeOptions();
	}

	/**
	 * Creates the CLI options for the merge command.
	 * @return Options for the merge command.
	 */
	private static Options createMergeOptions()
	{
		Options options = new Options();

		Option opt = new Option("o", false, "Set the output file.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("i", false, "Add an input file.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("t", true, "Set delta time (ns).");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("T", true, "Set the starting time (ns).");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("n", true, "Set maximum number of output events.");
		opt.setArgs(1);
		options.addOption(opt);

		opt = new Option("e", true, "Set number of events to merge in from each input file per merged event.  (Default is 1)");
		opt.setArgs(1);
		options.addOption(opt);
		
		opt = new Option("f", true, "Set input file list with format: [file_name],[n_reads_per_event],[start_time],[delta_time]" +
				"This option is not usable with the -i argument.");
		opt.setArgs(1);
		options.addOption(opt);
		
		return options;
	}

	/** 
	 * Parse the command line options for the merge command using apache CLI.
	 * @param argv The input arguments for the merge command. 
	 */
	public void parse(String[] argv) throws Exception
	{
		CommandLine cl = parser.parse(options, argv);

		// Must have at least one of -i or -f.
		if (!cl.hasOption("i") && !cl.hasOption("f"))
		{
			System.err.println("The merge command requires one of the -i or -f options.");
		}
		// Cannot have both -i and -f.
		else if (cl.hasOption("i") && cl.hasOption("f"))
		{
			System.err.println("The -i and -f options cannot be used together.");
			printUsage(true);
		}
		
		// Read a file containing comma-delimited list of fname and nreads.
		if (cl.hasOption("f"))
		{
			mergeFiles = createMergeFiles(FileUtil.loadFile(cl.getOptionValue("f")));
		}
		
		// Add input files one-by-one.
		if (cl.hasOption("i"))
		{	
			// Create input file array.
			infiles = FileUtil.createFiles(cl.getOptionValues("i"));
			
			// Create default merge files.
			mergeFiles = createDefaultMergeFiles(infiles);
		}

		// Output file.
		if (cl.hasOption("o"))
		{
			outfile = new File(cl.getOptionValue("o"));
		}
		else {
			outfile = new File("merged_events.slcio");
		}
		
		System.err.println("Set output file: " + outfile.getAbsolutePath());

		// Set the default time delta.
		if (cl.hasOption("t"))
		{
			def_dt = Float.parseFloat(cl.getOptionValue("t"));
		}

		// Set the default start time.
		if (cl.hasOption("T"))
		{
			def_startt = Float.parseFloat(cl.getOptionValue("T"));
		}

		// Set the maximum number of merged events.
		if (cl.hasOption("n"))
		{
			this.def_ntoread = Integer.parseInt(cl.getOptionValue("n"));
		}
	}

	/** 
	 * Execute the merge command with current arguments. 
	 */
	public void execute() throws Exception
	{
		MergeUtil.mergeFiles(outfile, mergeFiles, maxevents);
	}
	
	/**
	 * Create a list of @see MergeFileParameters objects from the
	 * input file specified with -f.
	 * @param lines File lines read from input file list (-f option).
	 * @return List of MergeFileParameters objects.
	 */
	public List createMergeFiles(List lines) throws IOException
	{	
		List mfiles = new ArrayList();
	
		for (Iterator iter = lines.iterator(); iter.hasNext();)
		{
			// Get the next line.	
			String line = (String)iter.next();
			
			// Split into fields on comma.
			String[] fields = line.split(",");
			
			if (fields.length != 0)
			{
				// Get file name.
				String fname = fields[0];
				
				// Get number of reads per event for this file.
				int nreads = def_ntoread;
				if (fields.length > 1)
					nreads = Integer.parseInt(fields[1]);
				
				// Get start time.
				float startt = def_startt;
				if (fields.length > 2)
					startt = Float.parseFloat(fields[2]);
				
				// Get delta time.
				float dt = def_dt;
				if (fields.length > 3)
					dt = Float.parseFloat(fields[3]);	
				
				// Create the input file.
				File f = new File(fname);
				
				// Create the merge file object.
				MergeFileParameters fopt = new MergeFileParameters(f, nreads, startt, dt);
				
				// Add this file with options to merge files list.
				mfiles.add(fopt);
			}
		}
		return mfiles;
	}
	
	/**
	 * Create the default merge options, which is one event from the file per merged output event.
	 * @param files An array of input File objects.
	 * @return A List of MergeFileParameters.
	 */
	public List createDefaultMergeFiles(File[] files) throws IOException
	{
		List m = new ArrayList();
		if (files != null)
		{
			for (int i=0; i<files.length;i++)
			{
				m.add(new MergeFileParameters(files[i], def_ntoread, def_startt, def_dt));
			}			
		}
		return m;
	}
	
	/**
	 * Print usage and (optionally) exit the program.
	 * @param doExit Whether or not to exit after printing usage.
	 */
	public void printUsage(boolean doExit)
	{
		HelpFormatter help = new HelpFormatter();

		help.printHelp("LCIO Merge command", "merge command", options, "");

		if (doExit)
		{
			System.exit(0);
		}
	}
}