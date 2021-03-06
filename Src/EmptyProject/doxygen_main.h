﻿/**
@mainpage PoolG's Adventure
@version Summer 2008

@section CodeName 개발코드명
- EP (EmptyProject)

@section Program 최종 프로그램명
- 풀쥐의 대모험(PoolG's Adventure)

@section intro 소개
- 본 프로젝트는 2008년 8월~9월 동안 연세대학교 프로그래밍 동아리 풀씨(PoolC)에서 진행된 프로젝트입니다.
- 팀장은 08학번 백민석, 팀원은 08학번 어윤욱, 06학번 김영신, 06학번 김재우입니다.
- 멘토는 04학번 김거엽입니다.

@section firstreader 이 문서를 처음 보시는 분들을 위한 도움말
이 문서는 PoolG 개발에 참여할 분들을 위해 작성되었습니다. PoolG는 간단한 로직이 구현된 프로그램이지만,
두 명 이상의 사람이 참여해서 개발하였으므로 프로그램 전반에 전제되는 규칙이 거의 없습니다.
또한 어렵지 않지만 처음 참여하시는 분들에게는 복잡할 수 있는 컴파일 환경이 필요합니다.
본 문서는 그러한 부분을 보완하고자 작성되었으므로 이 글을 처음 보시는 분들은 아래의 위치에서 프로그램 전체 틀을
이해하실 수 있으리라 생각합니다.

-# <a href="svncheckoutreadme.html">SVN 저장고 체크아웃하기</a>
-# <a href="buildreadme.html">빌드 및 실행 방법</a>
-# <b>클라이언트 프로그래밍</b>\n
클라이언트 프로그래밍에 관심이 있으신 분은 클라이언트 프로그램의 main() 함수가 작성된
EmptyProject.cpp을 먼저 살펴보시기 바랍니다.
-# <b>서버 프로그래밍</b>\n
서버 프로그래밍에 관심이 있으신 분은 서버 프로그램의 main() 함수가 작성된
EpServer.cpp를 먼저 살펴보시기 바랍니다.

@section FIRSTCREATEINFO 최초 문서 작성 정보
- 작성자 : gb
- 작성일 : 2008/09/10

@section MODIFYINFO 최종 문서 수정 정보
- 수정내역 : gb(2008/09/10) --- doxygen 문서 작성 시작



*/



/**
@page BuildReadMe 빌드 및 실행 방법


<h3>작성: gb 2008-09-10</h3>

만일 EP 소스 코드를 체크아웃하지 않은 분들은 <a href="svncheckoutreadme.html">SVN 저장고 체크아웃하기</a> 페이지를 참고해 주십시오.

<br>
<br>

<h2>1. PoolG 빌드를 하는데 필요한 프로그램</h2>

- Boost 라이브러리 (1_35_0 혹은 이상)
- RakNet 3.26
- DirectX SDK (June 2008) 혹은 그 이상... 이지만 해당 버젼 기준으로 작성됨
- Visual Studio 2008

<hr>

<h2>2. 프로그램 설정</h2>

- Boost 라이브러리 준비

Boost 라이브러리는 C++ 라이브러리이다. 이는 <SVN Root>/trunk/Thirdparties/boost_1_36_0.7z에

있으므로 만일 SVN 트렁크를 체크아웃했다면 이미 다운로드 받은 상태일 것이다.

이 파일을 적절한 곳에다가 압축을 푼다. "C:\boost_1_36_0"에 압축을 풀었다고 가정하겠다.

C:\boost_1_36_0 디렉토리에 boost 디렉토리가 있는지 확인한다.


- RakNet 3.26 준비

RakNet은 멀티플레이 게임 엔진이다. 누구나 소스를 다운로드 받을 수 있지만 상용게임에 사용하려면

라이센스비를 지불해야 한다. 이 라이브러리의 소스코드 역시 트렁크에 포함되어 있지만, 바이너리 버젼도

같이 들어있기 때문에 굳이 따로 컴파일할 필요는 없다.


- DirectX SDK 준비

이는 트렁크에 포함되어 있지 않다. Microsoft 사이트에서 다운로드 받아야하며 찾기는 쉬울 것이다.

원하는 곳에다가 설치한다. 본 문서는 설치 기본 디렉토리인 "C:\Program Files\Microsoft DirectX SDK (June 2008)"에

설치했다고 가정하겠다.


- Visual Studio 2008 준비

설치 단계에서는 특별히 언급할 것이 없다. (Visual C++을 설치하지 않은 것 제외)

VS2008을 실행시키고 두 가지 정도를 설정해야한다. 하나는 Boost 라이브러리 디렉토리 설정이고,

다른 하나는 Direct SDK 라이브러리 디렉토리 설정이다.

	-# VS2008을 실행한다.
	-# 상단 메뉴의 Tools -> Options를 실행한다.
	-# 좌측에서 Projects and Solutions를 찾아 확장시키고 VC++ Directories를 선택한다.
	-# 우측 상단에 'Show directories for' 항목을 Include Files로 바꾸면 아래에 인클루드 파일(.h)을 찾을
	디렉토리 목록이 나올 것이다.
	-# 여기에 C:\boost_1_36_0과 C:\Program Files\Microsoft DirectX SDK (June 2008)\Include를 추가해준다.
	-# 우측 상단에 'Show directories for' 항목을 Library Files로 바꾸면 아래에 라이브러리 파일(.lib)을 찾을
	디렉토리 목록이 나올 것이다.
	-# 여기에 C:\Program Files\Microsoft DirectX SDK (June 2008)\Lib\x86을 추가해준다.
	(본 프로젝트는 x86 시스템을 기반으로 작성되었다. x64에서는 컴파일되지 않는다.)
	Boost 라이브러리는 헤더 파일만 추가하면 되므로 여기에서 추가할 필요는 없다.
	-# OK를 눌러 창을 닫는다.\n


<hr>

<h2>3. 빌드(컴파일 & 링크)하기</h2>


PoolG는 몇 가지 프로젝트가 모여있는 형태인데 이러한 프로젝트를 한꺼번에 열 수 있는 방법이 있다.

그것은 바로 <SVN Root>/trunk/Build/PoolG.sln 파일을 여는 것이다.

이 파일을 VS2008로 연 후에 상단 메뉴에서 Build -> Build Solution을 선택하여 전체 빌드를 시작한다.

빌드 중 혹시 에러가 난다면 꼭 팀원에게 문의 바란다.

빌드가 완료되면 모든 바이너리가 생성되었다는 뜻이다.

<hr>

<h2>4. 실행하기</h2>

PoolG는 현재 서버/클라이언트 구조로 실행되도록 설계되어 있다. 반드시 서버를 실행할 필요는 없지만,

전체적인 빌드 성공을 확인하기 위해서는 아래 과정대로 해 보도록 한다.


- VS2008 솔루션 빌드 설정 확인
	-# 우선 VS2008로 <SVN Root>/trunk/Build/PoolG.sln 파일을 연다.
	-# 상단 메뉴의 Build -> Configuration Manager를 클릭한다.
	-# Active solution configuration 항목을 Debug로 설정한다. (이미 되어있으면 그대로 둔다.)
	-# Active solution platform 항목을 Win32로 설정한다. (이미 되어있으면 그대로 둔다.)


- 서버 실행하기
	-# VS2008의 Solution Explorer에서 EpServer를 찾아 마우스 우클릭을 한다.
	-# 컨텍스트 메뉴 항목 중 Set as StartUp Project를 클릭한다.
	-# Ctrl-F5로 EpServer를 실행시킨다. EpServer 콘솔 프로그램이 실행될 것이다.
	-# VS2008의 Solution Explorer에서 EmptyProject를 찾아 마우스 우클릭을 한다.


- 클라이언트 실행하기
	-# VS2008의 Solution Explorer에서 EmptyProject를 찾아 마우스 우클릭을 한다.
	-# 컨텍스트 메뉴 항목 중 Set as StartUp Project를 클릭한다.
	-# Ctrl-F5로 EmptyProject를 실행시킨다. EmptyProject 클라이언트 프로그램이 실행될 것이다.\n


서버와 클라이언트를 차례대로 실행시켰다면 우선 EpServer의 콘솔창에 사용자가 정상적으로 접속했다는

메시지가 떴는지 확인한다.

클라이언트 프로그램이 게임 플레이 상태일 경우(인트로가 끝난 경우)에 서버 콘솔 프로그램에서

move라고 치고 엔터를 쳐 본다. 만일 클라이언트 프로그램에서 주인공이 암흑으로 옮겨간다면 일단

서버-클라이언트 상호 동작이 정상적으로 일어나고 있는 것이다.

*/




/**
@page SvnCheckoutReadMe SVN 저장고 체크아웃하기


<h3>작성: gb 2008-09-11</h3>

EP 프로젝트는 SCM으로 SVN을 이용합니다. 현재 개발되고 있는 소스 코드를 다운로드 받으시려면

아래의 주소에서 체크아웃 하십시오.

@code
svn://poolc.org/poolg/trunk
@endcode

저장소는 익명 접근에 대해 읽기 전용이며, 계정을 가진 사용자에 대해서만 쓰기 권한이 주어집니다.

EP 프로젝트는 윈도우 시스템 기반입니다. 윈도우에서 가장 널리 쓰이는 TortoiseSVN 클라이언트를

사용하시면 SVN을 쉽게 이용하실 수 있습니다.

*/
