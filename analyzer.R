# analyzer.R
# Jon Herman
# Decoupled sampling for use with the R sensitivity library
# (You must have the R sensitivity library installed for this to work)
# Example usage:
# Rscript --vanilla --slave analyzer.R sobol 1000 myParameterFile.txt myModelOutput.txt 1 > sensitivityResults.txt
# (<method> can be "sobol", "morris", or "fast")

# Important: You must call analyzer with the same parameters that you used for sampler
# It is "decoupled", but it still goes through the whole sampling process in the analysis too.

echo = FALSE
args = (commandArgs(TRUE))
library("sensitivity")

# Parse command line parameters for different methods
if(length(args) < 4) {
  stop("At least 4 arguments required: <method> <numSamples> <paramFile> <resultsFile> <columnNumber> (optional)\n(Optional additional parameters for Method of Morris: <levels> <grid.jump>)");

} else if(args[1]=="morris") {
  
  # Method of Morris has additional optional parameters
  if(length(args) >= 6) {
    levels = strtoi(args[6]);
  } else {
    levels = 10;
  }

  if(length(args) >= 7) {
    grid.jump = strtoi(args[7]);
  } else {
    grid.jump = levels/2;
  }
}

# Number of samples
N = strtoi(args[2])

# Read upper/lower bounds from parameter file
paramBounds = read.table(args[3], stringsAsFactors=FALSE);
nParams = nrow(paramBounds);
model = NULL; # Model will be defined externally

# Model results file
resultsFile = args[4];

# Column number (First column = 1)
if(length(args) >= 5) {
  columnNumber = strtoi(args[5]);
} else {
  columnNumber = 1;
}

# Set the random seed to 1 for now
# The random seed only affects Method of Morris
set.seed(1);

if(args[1]=="sobol") {
  # Generate samples and scale according to parameter bounds
  m1 = matrix(runif(nParams*N), nrow=N);
  m1 = sweep(m1, MARGIN=2, paramBounds[,3] - paramBounds[,2], '*');
  m1 = sweep(m1, MARGIN=2, paramBounds[,2], '+');

  m2 = matrix(runif(nParams*N), nrow=N);
  m2 = sweep(m2, MARGIN=2, paramBounds[,3] - paramBounds[,2], '*');
  m2 = sweep(m2, MARGIN=2, paramBounds[,2], '+');
  
  # Perform the cross-sampling
  X1 = data.frame(m1);
  X2 = data.frame(m2);
  sa = sobol2002(model, X1, X2, nboot = 100);

} else if(args[1]=="morris") {
  factors = paramBounds[,1];
  design = list(type="oat", levels=levels, grid.jump=grid.jump);
  binf = paramBounds[,2];
  bsup = paramBounds[,3];
  sa = morris(model, factors, N, design, binf, bsup);
  
} else if(args[1]=="fast") {
  factors = paramBounds[,1];
  qtypes = vector();
  qtypes[1:length(paramBounds[,1])] = "qunif";
  
  qbounds = list();
  for(i in 1:length(paramBounds[,1])) {
    qbounds[[i]] = list(min = paramBounds[i,2], max = paramBounds[i,3]);
  }

  # Assume default values of M and omega from Saltelli (frequency values)
  sa = fast99(model, factors, N, q = qtypes, q.arg = qbounds);   
}

# Read the model results file and calculate the sensitivity indices
cutCommand = paste("cut -f", columnNumber, " ", resultsFile, sep="");
y = read.table(pipe(cutCommand));
y = data.matrix(y);
tell(sa, y)
sa