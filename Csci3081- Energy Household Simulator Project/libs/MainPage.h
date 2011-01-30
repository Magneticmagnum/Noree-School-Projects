/*!
 * \mainpage CS 3081w Fall 2010 
 * \section contents Table of Contents
 *
 * <ul>
 *     <li><a href="#authors">Authors
 *     <li><a href="#overview">Project Overview</a></li>
 *     <li><a href="#gets">What You Get</a></li>
*     <li><a href="#works">How it Works</a></li>
 *     <li>
 *         <a href="#build">How to Build It</a>
 *         <ul>
 *             <li><a href="#commands">The Make Commands</a></li>
 *         </ul>
 *     </li>
 *     <li><a href="#run">How to Run It</a></li>
 *     <li><a href="#examples">How to Run Examples</a></li>
 *     <li><a href="#log">Logging and Troubleshooting</a></li>
 
 * </ul>
 * 
 * \section authors Authors
 *
 * <p>Michelle Chorn         3718149</p>
 * <p>Timothy Luong          3778856</p>
 * <p>Chaichana Noree        4295362</p>
 * <p>Christopher Ziolkowski 3674260</p>

 * \section overview Project Overview
 *
 * <p>These files contain all the source code for Phase 3 of the
 * simulation project. Below are all the instructions you will
 * need to build, compile, and run our simulation, as well as
 * a description of how the simulator works
 * Also contained in this document are the 
 * descriptions of all the source code and test cases.</p>
 * 
 * \section get What You Get
 *
 * <p>In this file system you should have a set of Model, Dishwasher, Microwave,
 * Oven, Person, Refrigerator, Scheduler, Stove, Television, Toaster, lights,
 * Computer and Waterheater files (both the .h and .cpp for each), as well as
 * their tests and helper functions. You should then have a Doxyfile, Makefile,
 * simulator.cpp, and a README.</p>
 *
* \section works How it Works
 * 
 * <p>This is a discrete-time simulator, starting with a simple simulation
 *  framework that represents time in discrete "ticks" of some arbitrary size,
 *  such as one minute. On each tick, every model instance will be notified so
 *  that it may update its state. For example, if a refrigerator was turned on
 *  at t=(2010-08-23 19:05) and takes 3 minutes to complete a cycle, then it
 *  should turn off at t=(2010-08-23 19:08).</p>
 *
 *  <p>In addition to the time simulator, this phase adds support for two
 *  special kinds of files: model-specification files and person-script files.
 *  The model-specification file contains a list of models that will be used
 *  during the simulation.  In addition, it lets the user change attributes
 *  for a model.  For more information on the changable variables and their
 *  syntax, consult the desired model's page in this documentation. To see an
 *  example, look in the doc file where there are two included examples.  One
 *  is an exact replica of the script followed in Phase2. The other is a
 *  multi-person simulation.</p>
 *
 *  <p>The person script files also have a specific syntax that need to be
 *  followed.  For more information on this, consult the person's documentation.</p>
 *
 * \section build How to Build It
 *
 * <p>Once you have confirmed that you have all the files listed above, you may now build
 * the program using the various 'make' commands listed below (depending on what you
 * want to do). Make sure you are in the top level directory that has all of the files
 * in it before you run make, or else it will not compile correctly. The cxxtest
 * suite will need to be in located in the parent directory of the build directory.</p>
 *
 *
 * \subsection commands The Make Commands
 *
 * <pre>make</pre>
 * <p>Builds the Simulator executable file, runs the unit tests and coverage, and creates the documentation</p>
 * <pre>make simulator</pre>
 * <p>Builds the Simulator executable file only</p>
 * <pre>make run_tests</pre>
 * <p>Runs the unit tests only</p>
 * <pre>make test_coverage</pre>
 * <p>Builds and runs the test coverage only</p>
 * <pre>make doc</pre>
 * <p>Creates the documentation using Doxygen in a folder labeled 'html'</p>
 * <pre>make clean</pre>
 * <p>Deletes any and all files created by the Makefile</p>
 *
 * \section run How to Run It
 *
 * <p>In order to run the actual simulation process, you first must make sure you have either
 * made all or made the Simulator using the above make commands. Making the Simulator creates
 * an executable file in your directory called 'Simulator*'. </p>
 * 
 * <p>Once everything is created and confirmed, make sure you are in your top-level directory and
 * then type:</p>
 * <pre>./Simulator [-d daysToRun] model-spec person-script ...</pre>
 * <p>Where:</p><ul>
 * <li><p><i>[-d daysToRun]</i> is an optional variable specifying the number of simulation days
 * for which you would like to run the simulation. If none is specified, it will
 * run for a default of 10 days. </p></li>
 * <li><p><i> model-spec</i> is the location of the model specification file.</p></li>
 * <li><p><i> person-script ... </i> is the location(s) of the person script
 * files</p></li>
 * </ul>
 *
 * NOTE: The locations of the person-spec files cannot have spaces (" ") within
 * the path.
 *
 * \section examples How to Run Examples
 * <p>After the project has been made using the make command
 * (see <a href="#commands">above</a>) you can now run the examples.</p>
 * <p>To run the Phase2 example from the main folder, enter into the terminal:</p>
 * <p><tt>./simulator [-d numberOfDays] doc/Phase2-Sample/phase2_model_list
 *  doc/Phase2-Sample/phase2_script</tt></p>
*
 * <p>To run the multi-person example from the main folder, enter into the terminal:</p>
 * <p><tt>./simulator [-d numberOfDays] doc/Multi-Person-Sample/multi-person-models
 * doc/Multi-Person-Sample/homer-script doc/Multi-Person-Sample/marge-script</tt>
 *
 * \section log Logging and Troubleshooting
 * <p> Our program implements an extensive logging system that logs information
 * about every tick that passes.  This logging file can be found in the same
 * directory as the simulator executable and is called "simulation.log".</b>
 *
 * <p> There is a conflict on some machines between our logger (LOG4CXX) and
 * gcc.  If you experience an error building the program follow the following
 * steps:</p></ol>
 * <li>Open up the terminal.</li>
 * <li>Type 'module list' to see the list of modules that are loaded for this terminal</li>
 * <li>If you see the module 'soft/gcc/4.2', you need to unload it.</li>
 * <li>To unload the module, type 'module unload soft/gcc/4.2' and hit enter.</li>
 * <li>Within the same terminal, type 'eclipse &' to run eclipse.</li>
 *
 * <p>Make should work now, but this change only applies for the currently opened terminal.
 * If the terminal window is closed, you'll need to repeat the above steps.</p>
 * <p>If your machine does not have LOG4CXX installed, it can be installed using
 * the following command on Linux machines:</p>
 *
 * <tt>sudo apt-get install log4cxx</tt>
 *
 * Known issues / bugs:
 * gcov's reported line coverage for Person.cpp class is 94.04%.  The lines not being
 * executed are either brackets '{' '}' or return statements which are actually
 * returning a value.  Without those return statement values, the program will not run
 * properly nor compile correctly.
 */
