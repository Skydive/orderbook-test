#!/usr/bin/env bash
# For each test (.cin) file in ./tests
# Run with .cin and expect .cout
echo "Running tests..."
for i in tests/*.cin; do
    # Guard to break if no matching files are found
    [ -f "$i" ] || break

    CIN_FILE="$i"
    BASE_NAME=$(basename "$i" .cin)
    PATH_NAME=$(dirname "$i")
    COUT_FILE="$PATH_NAME/$BASE_NAME.cout"
    if [ -f "$COUT_FILE" ]; then
        # Expected result file doesn't exist
        echo -n "Running test: $BASE_NAME... "
    else
        echo "Skipping test: $BASE_NAME ($COUT_FILE doesn't exist)"
        continue
    fi
    # Run on each .cin and ignore CERR 
    RESULT=$(./bin/IcebergExecutable < $CIN_FILE 2>/dev/null)
    RESULT_STATUS=$?
    if [ $RESULT_STATUS -ne 0 ]; then
        echo "Failure! Execution Error!"
        echo "Status: $DIFF_RESULT"
        echo "Aborting..."
        diff --color <(echo "$RESULT") <(echo "$EXPECTED_RESULT")
        exit 1
    fi

    EXPECTED_RESULT=$(cat $COUT_FILE)
    DIFF_OUTPUT=$(diff --color <(echo "$RESULT") <(echo "$EXPECTED_RESULT"))
    DIFF_RESULT=$?
    if [ $DIFF_RESULT -eq 0 ]; then
        echo "Success!"
    else
        echo "Failure! Incorrect Output."
        echo "Status: $DIFF_RESULT"
        echo "Aborting..."
        diff --color <(echo "$RESULT") <(echo "$EXPECTED_RESULT")
        exit 1
    fi
done
echo "All tests completed successfully!"
