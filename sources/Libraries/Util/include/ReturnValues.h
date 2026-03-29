/* -------------------------------------------------------------------------- *
   WinUnit - Maria Blees (maria.blees@microsoft.com)
 * -------------------------------------------------------------------------- */
/**
 *  @file ReturnValues.h
 *  This file contains the possible exit values of the program.
 */
#pragma once

// These are the possible return values for the application.

/// No errors occurred.
#define WINUNIT_EXIT_SUCCESS             0

/// At least one test failed (includes handled structured exceptions).
#define WINUNIT_EXIT_TEST_FAILURE        1

/// The application terminated abnormally (unhandled exception, abort(),
/// terminate()).
#define WINUNIT_EXIT_UNHANDLED_EXCEPTION 2 

/// There was a command-line usage error.
#define WINUNIT_USAGE_ERROR             -1
