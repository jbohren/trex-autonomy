#!/usr/bin/env python

# System modules
import os

def trex_path(tail_path):
  # Check if TREX path is defined
  if os.environ.has_key('TREX'):
    trex_home = os.environ['TREX']
  else:
    os.environ['TREX'] = os.path.abspath(os.path.join(__file__,"../","../","../"))
  
  # Compose path
  return os.path.join(trex_home,tail_path)
