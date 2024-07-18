from common import *
import faker

def test_not_optimized(env):
    """Tests the optimized version of the NOT iterator, which holds an optimized
    wildcard iterator which is its "reference" traversal iterator instead of the
    basic 'incremental iterator'"""

    conn = getConnectionByEnv(env)

    # Create an index that optimizes the wildcard iterator
    env.expect('FT.CREATE', 'idx', 'INDEXALL', 'ENABLE', 'SCHEMA', 't', 'TEXT').ok()

    n_docs = 1005       # 5 more than the amount of entries in an index block
    fake = faker.Faker()
    names = set()
    for i in range(n_docs):
        # Add a name to the list of names
        old_len = len(names)
        while len(names) == old_len:
            new_name = fake.name()
            names.add(new_name)

        conn.execute_command('HSET', f'doc{i}', 't', new_name)

    names = list(names)

    res = env.cmd('FT.SEARCH', 'idx', '-t | t', 'LIMIT', '0', '0')
    env.assertEqual(res, [n_docs])

    res = env.cmd('FT.SEARCH', 'idx', '-@t:123', 'LIMIT', '0', '0')
    env.assertEqual(res, [n_docs])

    res = env.cmd('FT.SEARCH', 'idx', f'-@t:{names[0]}', 'LIMIT', '0', '0')
    env.assertEqual(res, [n_docs-1])




    # TODO: Test the "ismissing(@t) | -ismissing(@t)" query that failed before.
    # Missing is the only thing that seems to be incompatible currently. Understand why.
    # In general, once all tests pass when the default behavior is the optimized one -> We're fully backward compatible.


