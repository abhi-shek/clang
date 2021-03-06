// RUN: %clang_cc1 -triple x86_64-linux-gnu -O1 %s -emit-llvm -o - | FileCheck %s
//
// Test generating of TBAA metadata for accesses to members of base classes.

struct A {
  int x, y, z;
};

struct B : A {
  int i;
};

struct C {
  int i;
  B b;
  int j;
};

int f1(B *b) {
// CHECK-LABEL: _Z2f1P1B
// CHECK: load i32, {{.*}}, !tbaa [[TAG_A_y:!.*]]
  return b->y;
}

int f2(C *c) {
// CHECK-LABEL: _Z2f2P1C
// CHECK: load i32, {{.*}}, !tbaa [[TAG_A_y]]
  return (&(c->b))->y;
}

struct D : virtual A
{};

struct E {
  D d;
};

int f3(D *d) {
// CHECK-LABEL: _Z2f3P1D
// CHECK: load i32, {{.*}}, !tbaa [[TAG_A_y]]
  return d->y;
}

int f4(E *e) {
// CHECK-LABEL: _Z2f4P1E
// CHECK: load i32, {{.*}}, !tbaa [[TAG_A_y]]
  return (&(e->d))->y;
}

// CHECK-DAG: [[TYPE_char:!.*]] = !{!"omnipotent char", {{.*}}, i64 0}
// CHECK-DAG: [[TYPE_int:!.*]] = !{!"int", [[TYPE_char]], i64 0}
// CHECK-DAG: [[TYPE_A:!.*]] = !{!"_ZTS1A", [[TYPE_int]], i64 0, [[TYPE_int]], i64 4, [[TYPE_int]], i64 8}
// CHECK-DAG: [[TAG_A_y]] = !{[[TYPE_A]], [[TYPE_int]], i64 4}
