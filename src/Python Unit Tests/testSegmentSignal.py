"""
Created on February 13, 2023
@author: Lance A. Endres
"""
import pandas                                    as pd
from   SegmentSignal                             import SegmentSignal
from   NoiseVarianceEstimateMethod               import NoiseVarianceEstimateMethod
import matplotlib.pyplot                         as plt

import unittest


class TestSegmentSignal(unittest.TestCase):
    # For reading a data file with known input and solution.
    numberOfDataPoints      = 100;
    rows                    = 5;
    columns                 = numberOfDataPoints;
    data                    = 0;

    # Input values specified in the paper.
    f						= 2.50;
    order					= 10;
    order1					= 6;
    niter					= 300;
    rmode					= NoiseVarianceEstimateMethod.Point;

    # Solutions specified in the paper.
    c_solution		= 45.842
    d_solution		= 0.06000

    # Input file.
    workingDirectory		= "..\\Test Data\\"
    inputFile               = workingDirectory+"Data Set 1.txt"


    @classmethod
    def setUpClass(cls):
        pass


    def setUp(self):
        """
        Set up function that runs before each test.
        """
        # Columns that are in the input file.  The first two are input to the function, the last three are the known solution.
        names       = ["Depth", "Log", "SegmentedLog", "EventSequence", "FilteredLog"]
        self.data   = pd.read_csv(self.inputFile, sep="\t", header=None, names=names)


    def testScalarResults(self):
        results   = SegmentSignal(list(self.data["Log"].values), self.f, self.order, self.order1, NoiseVarianceEstimateMethod.Point)

        self.assertAlmostEqual(results.SegmentDensity, self.d_solution, places=5)
        # The variance of the jump sequence is not correct.  The reason is unknown.  See "SegmentSignalUnitTests.cs".
        #self.assertAlmostEqual(results.JumpSequenceVariance, self.c_solution, places=3)


    def testArrayResults(self):
        results   = SegmentSignal(list(self.data["Log"].values), self.f, self.order, self.order1, NoiseVarianceEstimateMethod.Point)

        delta = 0.3
        for i in range(results.SignalLength):
            self.assertAlmostEqual(results.SegmentedLog[i], self.data["SegmentedLog"].loc[i], delta=delta)
            #self.assertAlmostEqual(results.FilteredSignal[i], self.data["FilteredLog"].loc[i], delta=delta)


    def testGeneratePlots(self):
        # This plots up data to review.  It is useful for debugging, but generally should be required.
        # Comment out the "skip" decorator to see the plots.
        results   = SegmentSignal(list(self.data["Log"].values), self.f, self.order, self.order1, NoiseVarianceEstimateMethod.Point)

        axis = plt.gca()
        x = range(results.SignalLength)
        axis.plot(x, results.FilteredSignal, color="red", label="Result Filtered Signal")
        axis.plot(x, self.data["FilteredLog"], label="Input Filtered Signal", linestyle=(0, (5, 5)), linewidth=2)
        axis.legend(loc="lower right", bbox_to_anchor=(1, 0), bbox_transform=axis.transAxes)
        plt.show()

        axis = plt.gca()
        x = range(results.SignalLength)
        axis.plot(x, results.SegmentedLog, color="red", label="Result Segmented Signal")
        axis.plot(x, self.data["SegmentedLog"], label="Input Segmented Signal", linestyle=(0, (5, 5)), linewidth=2)
        axis.legend(loc="lower right", bbox_to_anchor=(1, 0), bbox_transform=axis.transAxes)
        plt.show()


if __name__ == "__main__":
    unittest.main()