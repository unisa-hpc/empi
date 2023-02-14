#!/usr/bin/python3
import common
from common import run_experiment
from common import make_lulesh_command
from common import parse_lulesh
    
if __name__ == '__main__':
	p = common.base_args()
	p = common.common_args(p)
	p.add_argument("--lulesh_path",
                 type=str,
                 default="../examples/LULESH/build/",
                 help="Path to the LULESH folder (default: ../examples/LULESH/build/)"
                 )
	args = p.parse_args()
	print(args)

	run_experiment(args, "LULESH", make_lulesh_command(args), parse_lulesh)
