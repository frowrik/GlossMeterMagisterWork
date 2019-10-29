

typedef  void	(*TabFunctCall)();
struct TabList {
	const char *				tabName;
	TabFunctCall				guiHandler;
};


class IMGUITAB {
public:
	void		Init();
	void		AddTab(PConstStr Name, TabFunctCall Funct);


	void		DrawAuto(f32 Weig);
public:
	TabList		TabLists[30];
	u32			TabCounts;
	int			selectedTab;
};
