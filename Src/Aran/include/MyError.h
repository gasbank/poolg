// MyError.h
// 2008 Geoyeob Kim (gasbank@gmail.com)
#pragma once

enum MyErrorEnum
{
	MEE_FILEDESCRIPTOR_CORRUPTED,
	MEE_TERMINALSTRING_CORRUPTED,
	MEE_UNSUPPORTED_NODE,
	MEE_UNDEFINED_ERROR,
	MEE_SKELETON_BONES_ERROR,
	MEE_HIERARCHY_FRAMES_ERROR,
	MEE_BOOL_DATA_PARSE,
	MEE_NOT_A_MESH_TYPE,
	MEE_RTTI_INCONSISTENCY,
	MEE_DEVICE_NOT_READY,
	MEE_NOT_IMPLEMENTED_YET,
	MEE_ANIM1NODE_CORRUPTED,
	MEE_SKELETON1NODE_CORRUPTED,
	MEE_NODE_NOT_FOUND,
	MEE_UNSUPPORTED_ARNFILE,
	MEE_STL_INDEX_OUT_OF_BOUNDS,
	MEE_UNSUPPORTED_CURVENAME,
	MEE_GENERAL_VERIFICATION_FAILED,
};

class MyError
{
public:
	MyError(MyErrorEnum lee)
	{
		switch (lee)
		{
		case MEE_FILEDESCRIPTOR_CORRUPTED:	m_str = _T("File descriptor in ARN file is corrupted");								break;
		case MEE_TERMINALSTRING_CORRUPTED:	m_str = _T("Terminal string in ARN file is corrupted");								break;
		case MEE_UNDEFINED_ERROR:			m_str = _T("Undefined error occurred!");											break;
		case MEE_UNSUPPORTED_NODE:			m_str = _T("Unsupported or not implemented node detected; skip this node...");		break;
		case MEE_SKELETON_BONES_ERROR:		m_str = _T("Skeleton node's bone count error");										break;
		case MEE_HIERARCHY_FRAMES_ERROR:	m_str = _T("Hierarchy node's MyFrame count error");									break;
		case MEE_BOOL_DATA_PARSE:			m_str = _T("Error while parsing BOOL data type from ARN file");						break;
		case MEE_NOT_A_MESH_TYPE:			m_str = _T("Non-mesh type enum was set at mesh type object");						break;
		case MEE_RTTI_INCONSISTENCY:		m_str = _T("Enum based RTTI inconsistent! Type-conversion failed");					break;
		case MEE_DEVICE_NOT_READY:			m_str = _T("Direct3D Renderer device is not ready");								break;
		case MEE_NOT_IMPLEMENTED_YET:		m_str = _T("Reached to unimplemented area");										break;
		case MEE_ANIM1NODE_CORRUPTED:		m_str = _T("Anim1 data corrupted");													break;
		case MEE_SKELETON1NODE_CORRUPTED:	m_str = _T("Skeleton1 data corrupted");												break;
		case MEE_NODE_NOT_FOUND:			m_str = _T("Desired node not found");												break;
		case MEE_UNSUPPORTED_ARNFILE:		m_str = _T("Unsupported ARN file format");											break;
		case MEE_STL_INDEX_OUT_OF_BOUNDS:	m_str = _T("STL data structure: Index out of bounds");								break;
		case MEE_UNSUPPORTED_CURVENAME:		m_str = _T("Unsupported IPO curve name");											break;
		case MEE_GENERAL_VERIFICATION_FAILED: m_str = _T("General verification failed. Check your code");						break;
		default:							m_str = _T("<Should not see me! Never!>");											break;
		}
	}
	const TCHAR* toString() const { return m_str; }
private:
	TCHAR* m_str;
};
