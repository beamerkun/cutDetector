#!/usr/bin/env python

from __future__ import print_function
import argparse
import re
from sys import stdout

def strip_frame_pair(pair):
    return re.sub(r'.*;(.*)\].*', r'\1', pair)

def read_frames(filename):
    frames = set()
    with open(filename, "r") as f:
        for line in f:
            frames.add(line.rstrip())
    return frames

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("reference", help="a file with the reference frames")
    parser.add_argument("cuts", help="a file with the detected cuts")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    reference_set = read_frames(args.reference)
    reference_set2 = set(strip_frame_pair(x) for x in reference_set)

    cuts_set = read_frames(args.cuts)
    cuts_set2 = set(strip_frame_pair(x) for x in cuts_set)

    print("True-positives: ", len(cuts_set2.intersection(reference_set2)))
    print("False-positives: ", len(cuts_set2 - reference_set2))
    print("False-negatives: ", len(reference_set2 - cuts_set2))

    if args.verbose:
        if stdout.isatty():
            print("\nFalse-positives:")
            for i in cuts_set - reference_set: # TU NIE DZIALA
                print("  ", i)
            print("\nFalse-hits:")
            for i in reference_set - cuts_set: # TU TEZ
                print("  ", i)
        else:
            for i in cuts_set - reference_set: # TU NIE DZIALA
                print(i)
            for i in reference_set - cuts_set: # TU TEZ
                print(i)

main()
