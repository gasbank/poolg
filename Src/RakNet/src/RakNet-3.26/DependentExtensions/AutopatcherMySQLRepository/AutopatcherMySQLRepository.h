/// \file
/// \brief An implementation of the AutopatcherRepositoryInterface to use MySQL to store the relevant data
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __MYSQL_REPOSITORY_H
#define __MYSQL_REPOSITORY_H

#include "AutopatcherRepositoryInterface.h"
#include "MySQLInterface.h"
#include "Export.h"

class FileListProgress;

/// \ingroup Autopatcher
///  An implementation of the AutopatcherRepositoryInterface to use MySQL to store the relevant data
class RAK_DLL_EXPORT AutopatcherMySQLRepository : public AutopatcherRepositoryInterface, public MySQLInterface
{
public:
	AutopatcherMySQLRepository();
	~AutopatcherMySQLRepository();

	/// Create the tables used by the autopatcher, for all applications.  Call this first.
	/// \return True on success, false on failure.
	bool CreateAutopatcherTables(void);

	/// Destroy the tables used by the autopatcher.  Don't call this unless you don't want to use the autopatcher anymore, or are testing.
	/// \return True on success, false on failure.
	bool DestroyAutopatcherTables(void);

	/// Add an application for use by files.  Call this second.
	/// \param[in] applicationName A null terminated string.
	/// \param[in] userName Stored in the database, but otherwise unused.  Useful to track who added this application.
	/// \return True on success, false on failure.
	bool AddApplication(const char *applicationName, const char *userName);

	/// Remove an application and files used by that application.
	/// \param[in] applicationName A null terminated string previously passed to AddApplication
	/// \return True on success, false on failure.
	bool RemoveApplication(const char *applicationName);

	/// Update all the files for an application to match what is at the specified directory.  Call this third.
	/// Be careful not to call this with the wrong directory.
	/// This is implemented in a Begin and Rollback block so you won't a messed up database from get partial updates.
	/// \param[in] applicationName A null terminated string previously passed to AddApplication
	/// \param[in] applicationDirectory The base directory of your application.  All files in this directory and subdirectories are added.
	/// \param[in] userName Stored in the database, but otherwise unused.  Useful to track who added this revision
	/// \param[in] cb Callback to get progress updates. Pass 0 to not use.
	/// \return True on success, false on failure.
	bool UpdateApplicationFiles(const char *applicationName, const char *applicationDirectory, const char *userName, FileListProgress *cb);

	/// Get list of files added and deleted since a certain date.  This is used by AutopatcherServer and not usually explicitly called.
	/// \param[in] applicationName A null terminated string previously passed to AddApplication
	/// \param[out] addedFiles A list of the current versions of filenames with SHA1_LENGTH byte hashes as their data that were created after \a sinceData
	/// \param[out] deletedFiles A list of the current versions of filenames that were deleted after \a sinceData
	/// \param[in] An input date, in the string format of a timestamp.
	/// \param[out] currentDate The server date, in the string format of a timestamp.
	/// \return True on success, false on failure.
	virtual bool GetChangelistSinceDate(const char *applicationName, FileList *addedFiles, FileList *deletedFiles, const char *sinceDate, char currentDate[64]);

	/// Get patches (or files) for every file in input, assuming that input has a hash for each of those files.  This is used by AutopatcherServer and not usually explicitly called.
	/// \param[in] applicationName A null terminated string previously passed to AddApplication
	/// \param[in] input A list of files with hashes to get from the database.  If this hash exists, a patch to the current version is returned if this file is not the current version.  Otherwise the current version is returned.
	/// \param[out] patchList A list of files with either the filedata or the patch.  This is a subset of \a input.  The context data for each file will be either PC_WRITE_FILE (to just write the file) or PC_HASH_WITH_PATCH (to patch).  If PC_HASH_WITH_PATCH, then the file contains a SHA1_LENGTH byte patch followed by the hash.  The datalength is patchlength + SHA1_LENGTH
	/// \param[out] currentDate The current server date, in whatever format your repository uses
	/// \return True on success, false on failure.
	virtual bool GetPatches(const char *applicationName, FileList *input, FileList *patchList, char currentDate[64]);

	/// If any of the above functions fail, the error string is stored internally.  Call this to get it.
	virtual const char *GetLastError(void) const;

	/// Read part of a file into \a destination
	/// Return the number of bytes written. Return 0 when file is done.
	/// \param[in] filename Filename to read
	/// \param[in] startReadBytes What offset from the start of the file to read from
	/// \param[in] numBytesToRead How many bytes to read. This is also how many bytes have been allocated to preallocatedDestination
	/// \param[out] preallocatedDestination Write your data here
	/// \return The number of bytes read, or 0 if none
	virtual unsigned int GetFilePart( char *filename, unsigned int startReadBytes, unsigned int numBytesToRead, void *preallocatedDestination, FileListNodeContext context);
};
#endif
