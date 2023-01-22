	%SEGMENTSIGNALS Segments a signal into regions of assumed constant value.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL) Divides the SIGNAL into sections of constant
	%	value.  Returns the binary event sequence which is an array of zeros and ones.  The ones
	%	designate the boundaries calculated by the algorithm.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL, THRESHOLD) The THRESHOLD determines the
	%	sensitivity of the segmentation.  A lower value (0.01-1.0) leads to a frequently segmented
	%	output that closely resembles input.  A larger value (4.0-10.0) leads to fewer segments
	%	that only captures the major patterns.  The default is 3.5.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL, THRESHOLD, JUMPWINDOWSEQUENCESIZE) Length of
	%	the moving average window sized used for smoothing the input well log to arrive at an
	%	initial estimate of the jump sequence variance.  The default is 20.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL, THRESHOLD, JUMPWINDOWSEQUENCESIZE,
	%	NOISEVARIANCEWINDOWSIZE) length of the moving average window used for smoothing the noise
	%	variances.  The recommended value is approximately half of JUMPWINDOWSEQUENCESIZE.  The
	%	default is 10.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL, THRESHOLD, JUMPWINDOWSEQUENCESIZE,
	%	NOISEVARIANCEWINDOWSIZE, RMODE) The RMODE is the noise estimation option.
	%		RMODE = 0 ==> Point estimate of noise variance.
	%		RMODE = 1 ==> Noise estimates smoothed within segments.
	%	The default is 1.
	%
	%	BINARYEVENTSEQUENCE = SEGMENTSIGNALS(SIGNAL, THRESHOLD, JUMPWINDOWSEQUENCESIZE,
	%	NOISEVARIANCEWINDOWSIZE, RMODE, MAXSMLRITERATIONS) Uses MAXSMLRITERATIONS as the
	%	upper bound on the number of Single Most Likelihood Replacement iterations.  The number
	%	of iterations is proportional to the length of the input signal and inversely related
	%	to the THRESHOLD.  The default is 500.
	%
	%	[BINARYEVENTSEQUENCE FILTEREDSIGNAL] = SEGMENTSIGNALS(...) Returns the filtered signal.
	%
	%	[BINARYEVENTSEQUENCE FILTEREDSIGNAL SEGMENTEDSIGNAL] = SEGMENTSIGNALS(...) Returns the
	%	segmented signal as an array of constant values.
	%
	%	[BINARYEVENTSEQUENCE FILTEREDSIGNAL SEGMENTEDSIGNAL ESTIMATEDNOISEVARIANCE] = SEGMENTSIGNALS(...)
	%	Returns the estimated noise variance.