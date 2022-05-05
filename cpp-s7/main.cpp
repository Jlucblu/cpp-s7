#include "list.h"
#include "editor.h"
#include "log_duration.h"
#include "my_assert.h"
#include "stack_vector.h"
#include "translator.h"
#include "tests.h"
#include "simple_vector.h"
#include "array_ptr.h"
#include "MakeJosephusPermutation.h"
#include "SplitIntoSentences.h"


int main() {
    TestErrors();
    TestEditor();
    TestStackVector();
    TestSimple();
    Test1();
    Test2();
    TestReserveConstructor();
    TestReserveMethod();
    TestIntVector();
    TestAvoidsCopying();
    TestSplitting();
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    return 0;
}
