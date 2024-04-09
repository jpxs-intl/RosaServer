#!/bin/bash

nproc=$(nproc) || nproc=8

cmake --build . --parallel "${nproc}"