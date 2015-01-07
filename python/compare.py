#!/usr/bin/env python

from __future__ import print_function
import argparse
import re
from sys import stdout

class FalconPair(object):
    def __init__(self, a, b):
        self.a = a
        self.b = b
    def __eq__(self, other):
        return self.b == other.b
    def __lt__(self, other):
        return self.b < other.b
    def __hash__(self):
        return hash(self.b)
    def __str__(self):
        return "[{a};{b}]".format(
            a = self.a,
            b = self.b)

def split_frame_pair(pair):
    a,_,b = pair.lstrip("[").rstrip("]").partition(";")
    return FalconPair(a,b)

def read_frames(filename):
    frames = set()
    with open(filename, "r") as f:
        for line in f:
            frames.add(split_frame_pair(line.rstrip()))
    return frames

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("reference", help="a file with the reference frames")
    parser.add_argument("cuts", help="a file with the detected cuts")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    reference_set = read_frames(args.reference)

    cuts_set = read_frames(args.cuts)

    print("True-positives: ", len(cuts_set & reference_set))
    print("False-positives: ", len(cuts_set - reference_set))
    print("False-negatives: ", len(reference_set - cuts_set))

    if args.verbose:
        if stdout.isatty():
            print("\nFalse-positives:")
            for i in sorted (cuts_set - reference_set):
                print("  ", i)
            print("\nFalse-negatives:")
            for i in sorted (reference_set - cuts_set):
                print("  ", i)
        else:
            for i in sorted (cuts_set - reference_set):
                print(i)
            for i in sorted (reference_set - cuts_set):
                print(i)

main()
