#include "../resgroupcmds.c"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <utils/memutils.h>
#include "cmockery.h"

#define test_with_setup_and_teardown(test_func) \
	unit_test_setup_teardown(test_func, setup, teardown)

static ResourceOwner savedOwner = NULL;

void
setup(void **state)
{
	savedOwner = CurrentResourceOwner;
}

void
teardown(void **state)
{
	CurrentResourceOwner = savedOwner;
}

void
test__defaultResourceOwner_sets_when_CurrentResourceOwner_is_NULL(void **state)
{
	CurrentResourceOwner = NULL;
	ResourceOwner owner = defaultResourceOwner_Acquire("dummy");
	assert_true(CurrentResourceOwner == owner);
	defaultResourceOwner_Release(owner);
	assert_true(CurrentResourceOwner == NULL);
}

void
test__defaultResourceOwner_does_nothing_when_CurrentResourceOwner_is_set(void **state)
{
	const ResourceOwner initialOwner =
			ResourceOwnerCreate(NULL, "initial resource owner");
	CurrentResourceOwner = initialOwner;

	ResourceOwner owner = defaultResourceOwner_Acquire("dummy");
	assert_true(CurrentResourceOwner == initialOwner);
	defaultResourceOwner_Release(owner);
	assert_true(CurrentResourceOwner == initialOwner);

	ResourceOwnerDelete(initialOwner);
}

int
main(int argc, char *argv[])
{
	cmockery_parse_arguments(argc, argv);

	MemoryContextInit();
	TopTransactionContext =
			AllocSetContextCreate(TopMemoryContext,
			                      "mock TopTransactionContext",
			                      ALLOCSET_DEFAULT_MINSIZE,
			                      ALLOCSET_DEFAULT_INITSIZE,
			                      ALLOCSET_DEFAULT_MAXSIZE);

	const UnitTest tests[] = {
			test_with_setup_and_teardown(test__defaultResourceOwner_sets_when_CurrentResourceOwner_is_NULL),
			test_with_setup_and_teardown(test__defaultResourceOwner_does_nothing_when_CurrentResourceOwner_is_set),
	};

	run_tests(tests);
}
