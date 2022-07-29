from __future__ import annotations
import pycoacd
import typing

__all__ = [
    "coacd"
]


def coacd(input: str, output: str, log: str = '', threshold: float = 0.05, seed: int = 0, preprocess: bool = True, verbose: bool = True) -> int:
    """
    Approximate Convex Decomposition for 3D Meshes with Collision-Aware Concavity and Tree Search.

    Args:
      input: path for input mesh (.obj)
      output: path for output (.obj or .wrl)
      log: path for output logfile
      threshold: concavity threshold for terminating the decomposition (0.01~1)
      seed: random seed used for sampling
      preprocess: flag to enable manifold preprocessing. 
        If your input is already manifold mesh, disabling the preprocessing can avoid introducing extra artifacts.
      verbose: flag to enable console output.

    References:
      https://github.com/SarahWeiii/CoACD
    """
