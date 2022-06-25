/**
 * Martin Egli
 * 2022-06-26
 */
module arduino_nano() {
    color("Blue") {
        translate([0, 0, 0])
        cube([43.5, 18, 1]);
    }
    color("Silver") {
        translate([-1, 5, 1])
        cube([6, 8, 3]);
    }
    color("Silver") {
        translate([25, 6, 1])
        cube([4, 6, 2.5]);
    }
    color("Black") {
        translate([11, 9, 1])
        rotate([0, 0, -45])
        cube([7, 7, 1]);
    }
    color("Black") {
        translate([3, 0, -2.5])
        cube([38, 2.5, 2.5]);
        translate([3, 18-2.5, -2.5])
        cube([38, 2.5, 2.5]);
    }

}

module rotary_encoder(loc_res = 32) {
    color("Silver") {
        translate([0, 0, 0])
        cube([12, 12, 4]);
        translate([6, 6, 0])
        cylinder(d = 6, h = 10, $fn = loc_res);
        difference() {
            hull() {
                translate([6, 6, 10.5])
                cylinder(d = 6, h = 12, $fn = loc_res);
                translate([6, 6, 10.5+12])
                cylinder(d = 4, h = 1, $fn = loc_res);
            }
            translate([2, 0, 12.5])
            cube([8, 5, 12]);
        }
    }
}

module oled_module(loc_res = 32) {
    difference() {
        union() {
            color("DarkGray") {
                translate([0, 0, 0])
                cube([25, 27, 1]);
            }
            color("Silver") {
                translate([2, 2, -0.1])
                cylinder(d = 4, h= 1.2, $fn = loc_res);
                translate([25-2, 2, -0.1])
                cylinder(d = 4, h= 1.2, $fn = loc_res);
                translate([25-2, 27-2, -0.1])
                cylinder(d = 4, h= 1.2, $fn = loc_res);
                translate([2, 27-2, -0.1])
                cylinder(d = 4, h= 1.2, $fn = loc_res);
            }
        }
        translate([2, 2, -0.3])
        cylinder(d = 3, h= 1.6, $fn = loc_res);
        translate([25-2, 2, -0.2])
        cylinder(d = 3, h= 1.4, $fn = loc_res);
        translate([25-2, 27-2, -0.2])
        cylinder(d = 3, h= 1.4, $fn = loc_res);
        translate([2, 27-2, -0.2])
        cylinder(d = 3, h= 1.4, $fn = loc_res);
    }
    color("Black") {
        translate([0, 5, 1])
        cube([25, 16.5, 1.6]);
    }
    color("Black") {
        translate([15/2, 0, -2.5])
        cube([10, 2.5, 2.5]);
    }
    color("Orange") {
        translate([(25-12)/2, 21.5, 1])
        cube([12, 5, 1]);
    }
}
oled_module();

//rotary_encoder();
//arduino_nano();
