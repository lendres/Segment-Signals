"""
Created on February 14, 2023
@author: Lance A. Endres
"""
import numpy                                     as np
from   SegmentSignalPy                           import Segment
from   SegmentSignalPy                           import SegmentationResults
from   NoiseVarianceEstimateMethod               import NoiseVarianceEstimateMethod

class SegmentSignal():
    results     = None

    def Segment(
            self,
            signal,
            threshold,
            jumpSequenceWindowSize=10,
            noiseVarianceWindowSize=None,
            noiseVarianceEstimateMethod=NoiseVarianceEstimateMethod.Point,
            maxSMLRIterations=300
        ):
        """
        Signal Segmentation Algorithm of Radhakrishnan, et al.  The algorithm is useful for dividing
        a source signal into regions where the signal can be considered constant, but with noise. The
        boundaries of the regions and the value of the signal within each region are not known a priori.


        Parameters
        ----------
        signal : array like
            Input signal to be segmented.
        threshold : double
            Segmentation threshold.
        jumpSequenceWindowSize : integer, optional
            Length of the moving average window sized used for smoothing the input well log
            to arrive at an initial estimate of the jump sequence variance. The default is 10.
        noiseVarianceWindowSize : integer, optional
            Length of the moving average window used for smoothing the noise variances. It
            is recommended to be approximatly half of the jumpSequenceWindowSize.  If it is
            "None" the window will automatically be selected as half the jumpSequenceWindowSize
            The default is None.
        noiseVarianceEstimateMethod : NoiseVarianceEstimateMethod, optional
            Noise variance estimate option.
    		    0 : Point estimate of noise variance.
    		    1 : Noise estimates smoothed within segments.
            The default is NoiseVarianceEstimateMethod.Point.
        maxSMLRIterations : integer, optional
            Upper bound on the number of Single Most Likelihood Replacement iterations. The
            default is 300.

        Returns
        -------
        results : SegmentationResults
            Results of the segmentation.
        """
        # Handle options.
        if noiseVarianceWindowSize is None:
            noiseVarianceWindowSize = int(np.round(0.5*jumpSequenceWindowSize))

        results = Segment(signal, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize, noiseVarianceEstimateMethod, maxSMLRIterations)

        # Check error results and provide a message if an error occured.
        if results.Error < 0:
            raise Exception("An invalid event density estimated after threshold, reduce/increase f and rerun.")

        if results.Error > 0:
            message = "The logarithm argument became zero at sample number " + results.Error
            message += " during the calculation of likelihood ratios in Single Most Likelihood Replacement iterations.  "
            message += "There may be more samples of this type which may give rise to this problem, edit/rescale data values and rerun."
            raise Exception(message)

        self.results = results
        return results


    def PlotFileteredSignal(self, axis, xData, **kwargs):
        axis.plot(xData, self.results.FilteredSignal, color="cyan", label="Filtered Signal", **kwargs)

    def PlotSegmentedSignal(self, axis, xData, **kwargs):
        axis.plot(xData, self.results.SegmentedLog, color="red", label="Segmented Signal", **kwargs)

    def PlotBinaryEvents(self, axis, xData, **kwargs):
        ticks = axis.get_yticks()
        yData = [ticks[0], ticks[-1]]

        for i in range(self.results.SignalLength):
            if self.results.BinaryEventSequence[i]:
                axis.plot([xData[i], xData[i]], yData, color="mediumvioletred", **kwargs)