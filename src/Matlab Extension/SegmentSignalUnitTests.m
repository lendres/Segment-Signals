clc
fileID      = fopen("..\\Test Data\\Data Set 1.txt", "r");
formatSpec  = "%f %f %f %d %f";
A           = fscanf(fileID, formatSpec, [5 inf]);
A           = A';
fclose(fileID);
size(A)


signal    = [1 1 1 1 1];
threshold = 3.5;
order     = 20;
order1    = 10;
rmode     = 1;
niter     = 500;

[binaryEventSequence, filteredSignal, signalSegments, estimantedNoiseVariance] = SegmentSignalMex(signal, threshold, order, order1, rmode, niter)
