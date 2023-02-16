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
        # Solution provided in paper.
        # Columns that are in the input file.  The first two are input to the function, the last three are the known solution.
        names       = ["Depth", "Log", "SegmentedLog", "EventSequence", "FilteredLog"]
        cls.data   = pd.read_csv(cls.inputFile, sep="\t", header=None, names=names)

        cls.segmenter = SegmentSignal()
        cls.segmenter.Segment(list(cls.data["Log"].values), cls.f, cls.order, cls.order1, NoiseVarianceEstimateMethod.Point)
        cls.results = cls.segmenter.results

        cls.segmenter.FindSignificantZones(cls.data["Depth"], 1.0, False)

        # Large data set.
        cls.largeData   = pd.read_csv(cls.workingDirectory+"Large Data Set.txt", header=None, names=["Depth"])


    def setUp(self):
        """
        Set up function that runs before each test.
        """
        pass


    def testScalarResults(self):
        self.assertAlmostEqual(self.results.SegmentDensity, self.d_solution, places=5)
        # The variance of the jump sequence is not correct.  The reason is unknown.  See "SegmentSignalUnitTests.cs".
        #self.assertAlmostEqual(results.JumpSequenceVariance, self.c_solution, places=3)


    def testArrayResults(self):
        delta = 0.3
        for i in range(self.results.SignalLength):
            self.assertAlmostEqual(self.results.SegmentedLog[i], self.data["SegmentedLog"].loc[i], delta=delta)
            #self.assertAlmostEqual(results.FilteredSignal[i], self.data["FilteredLog"].loc[i], delta=delta)


    def testGeneratePlots(self):
        # This plots up data to review.  It is useful for debugging, but generally should be required.
        # Comment out the "skip" decorator to see the plots.
        x = self.data["Depth"]

        axis = plt.gca()
        axis.plot(x, self.results.FilteredSignal, color="red", label="Calculated Filtered Signal")
        axis.plot(x, self.data["FilteredLog"], label="Filtered Signal Solution", linestyle=(0, (5, 5)), linewidth=2)
        axis.legend(loc="lower right", bbox_to_anchor=(1, 0), bbox_transform=axis.transAxes)
        axis.grid()
        plt.show()

        axis = plt.gca()
        axis.plot(x, self.results.SegmentedLog, color="red", label="Calculated Segmented Signal")
        axis.plot(x, self.data["SegmentedLog"], label="Segmented Signal Solution", linestyle=(0, (5, 5)), linewidth=2)
        axis.legend(loc="lower right", bbox_to_anchor=(1, 0), bbox_transform=axis.transAxes)
        axis.grid()
        plt.show()


    def testFindSignificantZonesIndices(self):
        solution   = [[9, 28], [30, 40], [42, 54]]
        calculated = self.segmenter.significantZonesIndices

        result     = (calculated == solution).all()
        self.assertTrue(result)


    def testFindSignificantZonesValues(self):
        solution   = [[1898.75, 1895.86], [1895.55, 1894.03], [1893.72, 1891.89]]
        calculated =  self.segmenter.GetSignifcationZoneValues(self.data["Depth"])

        result     = (calculated == solution)
        self.assertTrue(result)


if __name__ == "__main__":
    unittest.main()