## Command-line wrapper for the R Sensitivity Analysis Library
### (To be used with Sobol, Method of Morris, and FAST Methods)

Note: you must have the sensitivity library installed for this to work. You can get the library [here](http://cran.r-project.org/web/packages/sensitivity/index.html). For more detailed installation instructions, see [this post](http://waterprogramming.wordpress.com/2012/09/19/starting-out-with-the-r-sensitivity-package/).

Parameter files should be created with 3 columns, name, lower bound, and upper bound, e.g.:
```
P1 0.0 1.0
P2 0.0 5.0
...etc.
```

Contents: 

* `sampler.R`: generate parameter samples. Example usage:
```
   Rscript --vanilla --slave sampler.R <method> <N> <parameterFile> > <sampleFile>
```

* `analyzer.R`: perform sensitivity analysis on model results. Example:
```
   Rscript --vanilla --slave analyzer.R <method> <N> <parameterFile> <modelOutputFile> <columnNumber> > <resultsFile>
```
   
* Sobol-G Function and other test functions. These are included just for examples of performing decoupled sensitivity analysis. (Sampling -> Evaluations -> Analysis).
   
Some notes:
* For each of the above files, `<method>` can be `sobol`, `morris`, or `fast`. Each of these gives a different output format during the analysis step.
* `<N>` is the number of original samples, which is not the same as the number of parameter sets in `sampleFile` (these are multiplied approximately by a factor of P, where there are P parameters)
* By default, samples and results will be printed to stdout. In the examples above they are piped into files.
* `<columnNumber>` starts at 1, not 0. If you do not provide a column number, the default is 1
   
The full process would look something like this:
```bash
module load R

# Perform sampling
Rscript --vanilla --slave sampler.R ${METHOD} ${N} ${PARAM_FILE} > ${NAME}.samples

# Run the model executable with these samples (assuming the model receives parameters over stdin and prints output to stdout, your mileage may vary).
./myModel < ${NAME}.samples > ${NAME}.modelout

# Perform the sensitivity analysis using this model output (column 1)
Rscript --vanilla --slave analyzer.R ${METHOD} ${N} ${PARAM_FILE} ${NAME}.modelout 1 > ${NAME}.results
```
