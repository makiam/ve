/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Pointers.h"

void TestFile(CuTest* tc)
{
	// File not found
	Stream* notFound = StreamCreateFromFile( AllocatorGetHeap(), "Test.h", StreamMode::Read );
	CuAssertPtrEquals(tc, nullptr, notFound);

	// File opening
	StreamPtr file( pStreamCreateFromFile(AllocatorGetHeap(), "file.txt", StreamMode::Read) );
	CuAssertPtrNotNull(tc, file);

	// File size.
	int64 size = StreamGetSize(file);
	CuAssertIntEquals(tc, 6, (int) size);

	// File read
	String text;
	StreamReadString(file, text);
	CuAssertStrEquals(tc, "foobar", text.c_str());

	// File seek
	StreamSetPosition(file, -3, StreamSeekMode::RelativeEnd);

	// File tell
	int64 offset = StreamGetPosition(file);
	CuAssertIntEquals(tc, 3, (int) offset);

	// File read
	StreamReadString(file, text);
	CuAssertStrEquals(tc, "bar", text.c_str());

	StreamPtr flines( pStreamCreateFromFile(AllocatorGetHeap(), "lines.txt", StreamMode::Read) );
	CuAssertPtrNotNull(tc, flines);

	// Read lines.
	std::vector<String> lines;
	StreamReadLines(flines, lines);
	CuAssertIntEquals(tc, 3, lines.size());
	CuAssertStrEquals(tc, "foo", lines[0].c_str());
	CuAssertStrEquals(tc, "bar", lines[1].c_str());
	CuAssertStrEquals(tc, "spam", lines[2].c_str());
}

void TestWeb(CuTest* tc)
{
	Stream* ws = StreamCreateWeb(AllocatorGetHeap(), "http://www.google.com", StreamMode::Read);

	String response;
	StreamReadString(ws, response);

	CuAssertTrue(tc, response.size() > 0);

	StreamDestroy(ws);
}

CuSuite* GetSuiteStreams()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestFile);
	SUITE_ADD_TEST(suite, TestWeb);
    return suite;
}