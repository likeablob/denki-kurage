include <BOSL2/std.scad>

// CYD (ESP32-2432S028) Dimensions
CYD_PCB_L = 86.0; // Long axis (X)
CYD_PCB_W = 50.0; // Short axis (Y)
CYD_PCB_D = 1.6;
CYD_PCB_SIZE = [CYD_PCB_L, CYD_PCB_W, CYD_PCB_D];

CYD_SCREEN_L = 69.4; // Long axis (X)
CYD_SCREEN_W = 50.0; // Short axis (Y)
CYD_SCREEN_D = 4.5;
CYD_SCREEN_SIZE = [CYD_SCREEN_L, CYD_SCREEN_W, CYD_SCREEN_D];

CYD_HOLE_D = 3.5;
CYD_HOLE_OFFSET = 3.8; // From corner center

module cyd_dummy() {
  color("yellow")
    cube(CYD_PCB_SIZE, anchor=BOTTOM);

  // Screen
  color("black")
    up(CYD_PCB_D)
      cube(CYD_SCREEN_SIZE, anchor=BOTTOM);

  // Mounting Holes
  color("white")
    xflip_copy(offset=(CYD_PCB_L / 2 - CYD_HOLE_OFFSET))
      yflip_copy(offset=(CYD_PCB_W / 2 - CYD_HOLE_OFFSET))
        cylinder(h=CYD_PCB_D + 0.2, d=CYD_HOLE_D, center=true);
}

// Enclosure for Denki-Kurage

FRAME_L = CYD_PCB_L + 1.0;
FRAME_W = CYD_PCB_W + 2.0;
FRAME_D = CYD_SCREEN_D;
FRAME_ROUNDING = 2.0;

STAND_L = 39.0;
STAND_ANGLE = 70.0;
CABLE_HOLE_WALL = 1.8; // Bottom thickness for the cable hole

SCREW_HOLE_D = 1.7; // For M2 self-tapping

module dk_icon() {
  resize([10, 0, 0], auto=true)
    import("denki-kurage.svg", center=true, convexity=1);
}

module dk_frame_2d() {
  rect([FRAME_L, FRAME_W], rounding=FRAME_ROUNDING);
}

module dk_stand_front_2d() {
  rect([STAND_L, FRAME_W], rounding=FRAME_ROUNDING);
}

module dk_stand_body(l = STAND_L) {
  difference() {
    rotate([0, 90, 0])
      cylinder(d=FRAME_W, h=l, center=true);

    // Cut the bottom half to create a half-cylinder (Kamaboko) shape
    down(FRAME_W / 2)
      cube([l + 1, FRAME_W + 1, FRAME_W], anchor=CENTER);
  }
}

module dk_stand(enable_ldr_hole = false) {
  color("#052a7a")
    difference() {
      union() {
        // Main Frame (X+ is top)
        difference() {
          // Main body
          linear_extrude(height=FRAME_D)
            dk_frame_2d();

          // Screen cutout
          down(0.1)
            linear_extrude(height=FRAME_D + 0.2)
              rect([CYD_SCREEN_L + 0.1, CYD_SCREEN_W + 0.1]);
        }

        // Bridge block to fill 1mm gap between frame and stand (X=10, 5mm overlap on frame side, 4mm on stand side)
        translate([-FRAME_L / 2, 0, 0])
          linear_extrude(height=FRAME_D)
            rect([10, FRAME_W], anchor=CENTER);

        // Stand Part (Offset 1mm from -X edge of the frame)
        difference() {
          translate([-(FRAME_L / 2 + 1.0), 0, 0]) {
            hull() {
              // Front Plate
              left(STAND_L / 2)
                linear_extrude(height=0.1)
                  dk_stand_front_2d();

              // Body (Half-Cylinder)
              left(STAND_L / 2)
                up(FRAME_D)
                  dk_stand_body(l=STAND_L);
            }
          }

          // Cutting Cube for Stand Angle
          translate([-(FRAME_L / 2 + 1.0 + STAND_L), 0, 0])
            rotate([0, 90 - STAND_ANGLE, 0])
              cube([100, FRAME_W + 10, 200], anchor=BOTTOM + RIGHT);
        }
      }

      // Screen cutout (flex)
      translate([-(CYD_SCREEN_L) / 2 + 0.01, -(CYD_SCREEN_W / 2) + 5, 1])
        cube([1.2, 9.1, FRAME_D], anchor=BOTTOM + RIGHT + FWD);

      // Subtractions: Mounting holes, cable hole, and other cutouts
      up(1.0)
        xflip_copy(offset=(CYD_PCB_L / 2 - CYD_HOLE_OFFSET))
          yflip_copy(offset=(CYD_PCB_W / 2 - CYD_HOLE_OFFSET))
            cylinder(h=FRAME_D + 10, d=SCREW_HOLE_D);

      // Space for cable (type-c) with uniform bottom thickness
      type_c_y_offset_from_center = -1.7;
      translate([CABLE_HOLE_WALL, type_c_y_offset_from_center, FRAME_D])
        hull() {
          // Top block (at Z = FRAME_D)
          translate([-(FRAME_L / 2 + STAND_L) + ( (FRAME_W / 2) * tan(90 - STAND_ANGLE)), 0, 0])
            cube([STAND_L, 13.5, FRAME_W / 2], anchor=BOTTOM + LEFT);

          // Bottom thin block (at Z = CABLE_HOLE_WALL)
          translate([-(FRAME_L / 2 + STAND_L) + (CABLE_HOLE_WALL * tan(90 - STAND_ANGLE)), 0, 0])
            cube([STAND_L, 13.5, 0.01], anchor=BOTTOM + LEFT);
        }

      // Space for LDR
      translate([(CYD_PCB_L) / 2 - 2, -(FRAME_W / 2) + 8.2, FRAME_D + 0.01])
        cube([4.3, 6.3, 1.5], anchor=TOP + RIGHT + FWD);

      // Hole for LDR
      if (enable_ldr_hole) {
        translate([(CYD_PCB_L) / 2 - 4.3, -(FRAME_W / 2) + 11.3, -0.01])
          linear_extrude(height=FRAME_D + 0.2)
            rect([3, 4], rounding=1);
      }
    }
  ;

  // Logo (Multi-color print support)
  color("white")
    translate([-(FRAME_L / 2 + 9), 0, -0.01]) {
      zrot(90)
        mirror([0, 1, 0]) {
          linear_extrude(height=0.5)
            text("電気くらげ", size=6, font="M Plus 1:style=Bold", halign="center", valign="center");

          // Rectangular frame (1mm wide)
          linear_extrude(height=0.5)
            difference() {
              shell2d(1)
                rect([43, 10.4], rounding=1);
            }

          // Icon
          translate([0, -18, 0])
            linear_extrude(height=0.5)
              dk_icon();
        }
    }
  ;
}
