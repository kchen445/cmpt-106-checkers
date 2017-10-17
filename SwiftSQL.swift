// SQLite interfacing with Swift.
// Swift uses the same SQLite library as C/C++ so the function call should 
// be the same. All this file is, is a series of functions for manipulating
// database tables.
//
// The testing table I was using connsisted of 3 columns,
//      ID | NAME | AGE

import Cocoa
import SQLite3 // In C this would be #include<sqlite3.h> I think.

// Open a database file.
// Returns a pointer to that database for use with other SQLite commands.
func openDatabase (_ file: String) -> OpaquePointer? {
    var db: OpaquePointer? = nil
    if sqlite3_open(String(file), &db) == SQLITE_OK {
        print("Open OK!")
        return db
    } else {
        print("Unable to open database.")
        return nil
    }
}

// SQLLite SELECT command.
// Prints the contents to stdout.
//
// *** Needs more work to be usable ***
func select (database: OpaquePointer?, _ column: String, from table: String, where whereText: String = "") {
    var selectStatement: OpaquePointer? = nil
    
    // Build SQLite command from parameters.
    let command: String
    if whereText != "" {
        command = "SELECT \(column) FROM \(table) WHERE \(whereText);"
    } else {
        command = "SELECT \(column) FROM \(table);"
    }
    
    // Prepare SQLite statement.
    if sqlite3_prepare(db, command, -1, &selectStatement, nil) == SQLITE_OK {
        
        print("Query Result:")

        // Execute statement.
        while sqlite3_step(selectStatement) == SQLITE_ROW {

            // Interperate the row from the statement.
            let id = sqlite3_column_int(selectStatement, 0)
            let name = String(cString: sqlite3_column_text(selectStatement, 1)!)
            let age = sqlite3_column_int(selectStatement, 2)
            
            print("\(id) | \(name) | \(age)")
        }
    } else {
        print("Statement could not be prepared")
    }
    
    // Finalize the statement.
    // Must be called (probably dealocates memory or something)
    sqlite3_finalize(selectStatement)
}

// SQLite update command.
func update (database: OpaquePointer?, table: String, set setText: String, where whereText: String) {
    var updateStatement: OpaquePointer? = nil
    let command: String = "UPDATE \(table) SET \(setText) WHERE \(whereText);"
    if sqlite3_prepare(database, command, -1, &updateStatement, nil) == SQLITE_OK {
        if sqlite3_step(updateStatement) == SQLITE_DONE {
            print("Successfully updated row.")
        } else {
            print("Could not update row.")
        }
    } else {
        print("UPDATE statement could not be prepared")
    }
    
    sqlite3_finalize(updateStatement)
}

// SQLite insert command.
func insert (database: OpaquePointer?, table: String, values: String) {
    var insertStatement: OpaquePointer? = nil
    let command: String = "INSERT INTO \(table) VALUES \(values)"
    
    if sqlite3_prepare(db, command, -1, &insertStatement, nil) == SQLITE_OK {
        if sqlite3_step(insertStatement) == SQLITE_DONE {
            print("Insert OK!")
        } else {
            print("Failed to insert row.")
        }
    } else {
        print("Unable to prepare command.")
    }
    
    sqlite3_finalize(insertStatement)
}


// Testing code
let db = openDatabase("/Users/Jeremy/Desktop/DatabaseTest/TestDB.db")
// Print the entire table.
select(database: db, "*", from: "people")

