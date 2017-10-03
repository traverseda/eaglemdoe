//------------------------------------------------------------------------------
// ControlEdges.pov
// Copyright (C) 2007-2008 Oliver Hamann.
//------------------------------------------------------------------------------

#include "math.inc"


camera {
	orthographic
	location <0, 0, 30>
	look_at <0, 0, 0>
	sky <0,-1,0>
}

light_source {
	<-1000, -1500, 1700>
	#declare light_value=1.0;
	color rgb <light_value,light_value,light_value>
}

sky_sphere {
	pigment {
		gradient y
		color_map {
			[ 0.0  color rgb <0.8,0.8,1> ]
			[ 0.5  color rgb <0.5,0.5,0.5> ]
			[ 1.0  color rgb <0.08,0.08,0> ]
		}
		scale 2
		translate -1
	}
}

#declare Quarter = intersection {
	merge {
		cone { <0,0,0> 0 <20,0,0> 1 rotate <0,0,atan2d(1,20)> }
		intersection {
			plane { <1,-20,0> 0 }
			plane { <0,1,0> 5 }
		}
	}
	plane { <-1,0,20*cosd(atan2d(1,20))> 0 }
	plane { <1,0,0> 0 rotate <0,-25,0> translate <19,0,0> }
	translate <-19.5,-5,0>
	scale <1,1,1>
}

object {
	merge {
		plane { <0,0,1> 0 }
		object { Quarter scale <-1,-1, 1> }
		object { Quarter scale <-1, 1, 1> }
		object { Quarter scale < 1,-1, 1> }
		object { Quarter scale < 1, 1, 1> }
	}
	pigment {
		color rgb <0.50, 0.50, 0.50>
	}
	finish {
		ambient 0.1
		brilliance 1
		diffuse 0.6
		metallic
		specular 0.8
		roughness 0.04
		reflection 0.5
	}
}
