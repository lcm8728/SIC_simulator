SIC/XE 머신 구현
================

1. 프로젝트 목표
----------------
	An Introduction To Systems Programming(Leland L. Beck, D.Manjula, 1997)에 소개된 SIC/XE의 일부를 구현하는 것을 목표로 한다.
	SIC/XE 머신을 구현하기 위해, shell을 구성하고, 가상 메모리 공간, assembler, linking loader 등의 명령어를 탑재하여 이를 수행한다.
	다음 프로젝트를 통하여 assembler, linking loader 등의 작동 원리를 익히는 것 뿐 아니라  메모리 공간을 직접 확인함으로서 컴퓨터 동작에 대한 직관적 이해를 돕는다.

2. 개발 환경
---------------
	이 프로그램은 gcc로 컴파일을 하며, 컴파일에 대한 명령어는 Makefile에 정의되어 있습니다.
	컴파일 및 실행 방법은 다음과 같습니다.
		$ make
		$ ./20120116.out

3. 사용 방법
	쉘에서 사용하는 명령어는 help, dir, quit, history, dump, edit, fill, reset, opcode, opcodelist가 있습니다.
	위 명령어 목록은 쉘에서 help 명령어로 볼 수 있습니다.

		sicsim> help // 명령어 목록을 보여줍니다.
		sicsim> dir  // 현재 디렉토리의 파일 및 하위 디렉토리를 보여줍니다.
		sicsim> quit // 프로그램을 종료합니다.
		sicsim> history // 입력 시점까지의 명령어 히스토리를 보여줍니다.
		sicsim> reset // 전체 메모리의 값을 0으로 초기화합니다.
		sicsim> opcodelist // opcode의 테이블을 보여줍니다.

	dump, edit, fill, opcode 명령어는 명령어 이외에 주소, 값 등의 인자를 받을 수 있습니다.
	인자들은 쉼표(comma)로 구분됩니다.

		sicsim> dump [start],[end] // 해당 메모리의 값을 보여줍니다.
		sicsim> edit addr, value   // 해당 주소의 메모리 값을 변경합니다.
		sicsim> fill start, end, value  // 해당 범위 주소의 메모리 값을 변경합니다.
		sicsim> opcode mnemonic    // 해당 mnemonic의 OPCODE를 보여줍니다.

	인자의 갯수는 쉼표의 갯수로 구분됩니다. 따라서 아래와 같은 경우는 오류로 처리합니다.
		
		ex)
		sicsim> dump , a  // 쉼표 앞에 아무런 인자도 없기 때문에 NULL 문자열을 받은 것으로 처리됩니다.
		sicsim> dump a,   // 쉼표 뒤에 아무런 인자도 없기 때문에 NULL 문자열을 받은 것으로 처리됩니다.
	
	또한, 인자가 너무 많거나 너무 적은 경우에도 오류로 처리합니다.
		
		ex)
		sicsim> fill aa, dd    // fill은 3개의 인자가 필요하기 때문에 오류로 처리됩니다.
		sicsim> edit aa, dd, d // edit은 2개의 인자가 필요하기 때문에 오류로 처리됩니다.
		sicsim> dump aa, bb,   // dump는 0 ~ 2개의 인자를 필요로 합니다. 이 경우 쉼표가 2개이기 때문에 인자가 3개인 것으로 인식합니다.
