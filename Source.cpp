#include<iostream>
#include<fstream>
using namespace std;

class Date;
class Post;
class User;
class AppNetwork;
class Helper;
class Comment;
class Controller;
class PostActivity;
class PostContent;
class Memory;

class Helper
{
public:
	static int length(char* cstring)
	{
		int len;
		for (len = 0; cstring[len] != '\0'; len++);
		return len;
	}
	static int constlength(const char* cstring)
	{
		int len;
		for (len = 0; cstring[len] != '\0'; len++);
		return len;
	}
	static char* shrinkarray(char* cstring, bool deleteString = 1)
	{
		int len = length(cstring);
		char* temp = new char[len + 1];
		for (int i = 0; i < len; i++)
		{
			temp[i] = cstring[i];
		}
		temp[len] = '\0';
		if (deleteString)
		{
			delete[] cstring;
		}
		return temp;
	}
	static void strcopy(const char* source, char*& dest)
	{
		int len = constlength(source);
		for (int i = 0; i < len; i++)
		{
			dest[i] = source[i];
		}
		dest[len] = '\0';
	}
	static bool compareText(char* str1, char* str2)
	{
		int len1, len2;
		for (len1 = 0; str1[len1] != '\0'; len1++);
		for (len2 = 0; str2[len2] != '\0'; len2++);
		if (len1 == len2)
		{
			for (int i = 0; i < len1; i++)
			{
				if (str1[i] != str2[i])
					return false;
			}
			return true;
		}
		else
			return false;

	}
};

class Date
{
private:
	int date;
	int month;
	int year;
public:
	Date();
	~Date();
	Date(int d, int m, int y) : date(d), month(m), year(y) {}
	static Date currentDate;
	void setValues(int d, int m, int y);
	void printDate();
	int getDay() { return date; }
	int getMonth() { return month; }
	int getYear() { return year; }
	bool isMemory(Date rhs);
	bool isDateWithinTwoDays(Date rhs);
};

class PostContent
{
public:
	virtual void print()
	{
		cout << "Default print called.\n";
	}
	virtual ~PostContent();
};

class PostActivity :public PostContent
{
private:
	int type;
	char* text;
public:
	PostActivity(int i, char* t);
	void print();
	~PostActivity();
};

class Comment
{
private:
	char* ID;
	char* text;
	AppNetwork* commentBy;
	Post* commentedOn;
	static int totalComments;
public:
	Comment(char* cID, Post* cOn, AppNetwork* cBy, char* text);
	//~Comment();
	void Print();
	char* getID() { return ID; }
	char* getText() { return text; }
	static void setTotalComments(int c) { totalComments = c; }
};

class Post
{
private:
	static int noOfPosts;
	char* postID;
	Date datePosted;
	char* text;
	AppNetwork* sharedBy;
	AppNetwork** likedBy;
	int likes;
	Comment** comments;
	int totalComments;
	PostContent** content;
public:
	Post();
	virtual ~Post();
	void ReadDataFromFile(ifstream& fin);
	void printPost();
	void setText(char* t) { Helper::strcopy(t, text); }
	void addComment(Comment* ptr) { comments[totalComments] = ptr; }
	void setLikedBy(AppNetwork* ptr) { likedBy[likes] = ptr; likes++; }
	void setSharedBy(AppNetwork* ptr) { sharedBy = ptr; }
	void setComments(Comment* ptr) { comments[totalComments] = ptr; }
	void incrementComments() { totalComments++; }
	void setPostContent(PostActivity* ptr) { content[0] = ptr; }
	virtual void viewPost();
	static void setTotalPosts(int P) { noOfPosts = P; }
	static int getPostTotal() { return noOfPosts; }
	int getTotalComments() { return totalComments; }
	char* getID() { return postID; }
	char* getText() { return text; }
	Comment** getCommentsList() { return comments; }
	Date getDate() { return datePosted; }
	AppNetwork** getLikedBy() { return likedBy; }
	AppNetwork* getSharedBy() { return sharedBy; }
	PostContent* getPostContent() { return content[0]; }
};

class Memory : public Post
{
private:
	Post* oldPostPtr;
public:
	Memory(Post* old, char* newText);
	void viewPost();
	~Memory();
};

class AppNetwork
{
protected:
	Post** timeline;
	int totalTimelinePosts;

public:
	AppNetwork()
	{
		timeline = new Post * [10];
		for (int i = 0; i < 10; i++)
		{
			timeline[i] = nullptr;
		}
		totalTimelinePosts = 0;
	}
	virtual void addToTimeline(Post* ptr)
	{
		timeline[totalTimelinePosts] = ptr;
		totalTimelinePosts++;
	}
	void incrementTimelineTotal() { totalTimelinePosts++; }
	virtual void PrintInfo() { ; }
	virtual void printUserName() { ; }
	void viewTimeLine()
	{
		for (int i = 0; i < 10; i++)
		{
			if (timeline[i])
			{
				timeline[i]->viewPost();
				cout << endl;
			}
		}
	}
	void PostComment(Post* p, char* t)
	{
		char tempID[] = "nPost";
		Comment* newComment = new Comment(tempID, p, this, t);
		p->addComment(newComment);
	}

	~AppNetwork();
};

class Page :public AppNetwork
{
	char* ID;
	char* Title;
	static int totalPages;
public:
	Page();
	void ReadDataFromFile(ifstream& fin);
	void PrintInfo();
	~Page();
	void addToTimeline(Post* ptr);
	void printUserName() { cout << Title; }

	static void setPageTotal(int P) { totalPages = P; }
	static int getPageTotal() { return totalPages; }
	char* getID() { return ID; }
	char* getTitle() { return Title; }
	int getTotalTimelinePosts() { return totalTimelinePosts; }
	Post* getTimeLinePost(int p) { return timeline[p]; }

};

class User :public AppNetwork
{
private:
	char* ID;
	char* Fname;
	char* Lname;
	User** FriendList;
	Page** LikedPages;
	int friendsTotal;
	int pagesTotal;
	static int totalusers;

public:
	User();
	void AddFriend(User*&);
	void ReadDataFromFile(ifstream& fin);
	void LikePage(Page*&);
	void ViewFriendList();
	void ViewLikedPages();
	void PrintInfo();
	void addToTimeline(Post* ptr);
	void printUserName() { cout << Fname << " " << Lname; }
	void viewHomePage(Date cDate);
	void SeeMemories();
	void shareMemory(Post* p, char* t);
	~User();

	static void setTotalUser(int U) { totalusers = U; }
	static int getTotalUsers() { return totalusers; }
	char* getID() { return ID; }
	char* getFname() { return Fname; }
	char* getLname() { return Lname; }
	User** getFriendList() { return FriendList; }
};

class Controller
{
private:
	User** AllUsers;
	Page** AllPages;
	Post** AllPosts;
public:
	User* SearchUserByID(char* id);
	Page* SearchPageByID(char* id);
	AppNetwork* SearchNetworkMemberByID(char* id);
	Post* SearchPostByID(char* id);
	void LoadAllUser(const char* file);
	void LoadAllPages(const char* file);
	void LoadAllPosts(const char* file);
	void LinkUsersAndPages(const char* file);
	void LoadAllComments(const char* file);
	void LoadAllActivities(const char* file);
	~Controller();

	void LoadData()
	{
		LoadAllUser("SocialNetworkUsers.txt");
		LoadAllPages("SocialNetworkPages.txt");
		LinkUsersAndPages("UsersFriendsAndLikedPages.txt");
		LoadAllPosts("SocialNetworkPosts.txt");
		LoadAllComments("SocialNetworkComments.txt");
		LoadAllActivities("SocialNetworkActivities.txt");
	}
	void Run()
	{
		User* currentUser = AllUsers[6];
		Post* currentPost = AllPosts[4];
		Page* currentPage = AllPages[0];
		Date systemDate = Date::currentDate;

		cout << currentUser->getFname() << " " << currentUser->getLname() << " successfully set as the current member.\n";

		cout << "System Date: ";
		systemDate.printDate();
		cout << endl << endl;

		currentUser->ViewFriendList();
		cout << endl;
		currentUser->ViewLikedPages();
		cout << endl;

		cout << "Viewing ";
		currentPost->printPost();
		currentPost->viewPost();

		cout << "\nList of people who have liked this post: \n";
		AppNetwork** likedByList = currentPost->getLikedBy();
		for (int i = 0; i < 10; i++)
		{
			if (likedByList[i])
			{
				likedByList[i]->PrintInfo();
			}
		}

		cout << endl;
		currentUser->printUserName();
		cout << " just liked this post! \n";
		currentPost->setLikedBy(currentUser);
		cout << "List of people who have liked this post now: \n";
		for (int i = 0; i < 10; i++)
		{
			if (likedByList[i])
			{
				likedByList[i]->PrintInfo();
			}
		}

		cout << endl << "Viewing timeline of " << currentUser->getFname() << " " << currentUser->getLname() << "\n";
		currentUser->viewTimeLine();

		cout << endl << "Viewing page of the current brand: \n";
		currentPage->viewTimeLine();

		cout << "Homepage:\n\n";
		currentUser->viewHomePage(systemDate);

		cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n\n";
		currentUser->SeeMemories();

		char bufferID[10] = "post10";
		char bufferText[80] = "Never thought i will be specialist in this field.";
		Post* memoryAddOnPost = SearchPostByID(bufferID);
		currentUser->shareMemory(memoryAddOnPost, bufferText);
		currentUser->incrementTimelineTotal();

		cout << endl << "Viewing timeline of " << currentUser->getFname() << " " << currentUser->getLname() << "\n";
		currentUser->viewTimeLine();

		char bufferComment[] = "Thanks for the wishes.\n";
		char bufferID2[] = "post8";
		Post* postToCommentOn = SearchPostByID(bufferID2);
		currentUser->PostComment(postToCommentOn, bufferComment);
		postToCommentOn->incrementComments();

		cout << endl << currentUser->getFname() << " " << currentUser->getLname() << " just added a new comment:\n";
		postToCommentOn->viewPost();
	}

};

//-------------Initializing Static Variables---------------//
int User::totalusers = 0;
int Page::totalPages = 0;
int Post::noOfPosts = 0;
int Comment::totalComments = 0;
Date Date::currentDate = Date(17, 4, 2024);

int main()
{
	{
		Controller mainApp;
		mainApp.LoadData();
		mainApp.Run();
	}
	cout <<"Memory Leaks: "<< _CrtDumpMemoryLeaks();
	return 0;
}

//-------------USER FUNCTIONS--------------//
User::User()
{
	ID = new char[5];
	Fname = new char[80];
	Lname = new char[80];
	FriendList = new User * [10];
	for (int i = 0; i < 10; i++)
		FriendList[i] = nullptr;			//EVEN THOUGH NEW WILL NOT BE CREATED FOR THIS PURPOSE(bcs we'll just copy adress), MEMORY MUST BE ALLOCATED FOR 10
	LikedPages = new Page * [10];
	for (int i = 0; i < 10; i++)
		LikedPages[i] = nullptr;
	pagesTotal = friendsTotal = 0;
}
void User::ReadDataFromFile(ifstream & fin)
{
	fin >> ID;
	ID = Helper::shrinkarray(ID);
	fin >> Fname;
	Fname = Helper::shrinkarray(Fname);
	fin >> Lname;
	Lname = Helper::shrinkarray(Lname);

}
void User::ViewFriendList()
{
	cout << this->getFname() << " " << this->getLname();
	cout << " - Friend List:\n\n";
	for (int i = 0; i < 10; i++)
	{
		if (FriendList[i])
		{
			FriendList[i]->PrintInfo();
		}
	}
}
void User::ViewLikedPages()
{
	cout << this->getFname() << " " << this->getLname();
	cout << " - Liked Pages:\n\n";
	for (int i = 0; i < 10; i++)
	{
		if (LikedPages[i])
		{
			LikedPages[i]->PrintInfo();
		}
	}
}
void User::PrintInfo()
{
	cout << ID << " - " << Fname << " " << Lname << endl;
}
void User::AddFriend(User * &newFriend)
{
	if (friendsTotal < 10)
	{
		FriendList[friendsTotal] = newFriend;
		friendsTotal++;
	}
	else
		cout << "Error adding a new friend: Maximam Friend limit reached.\n";

}
void User::LikePage(Page * &likedPage)
{
	if (friendsTotal < 10)
	{
		LikedPages[pagesTotal] = likedPage;
		pagesTotal++;
	}
	else
		cout << "Error liking the page: Maximam Page limit reached.\n";

}
void User::addToTimeline(Post * ptr)
{
	timeline[totalTimelinePosts] = ptr;
	totalTimelinePosts++;
}
void User::viewHomePage(Date cDate)
{
	for (int i = 0; i < totalTimelinePosts; i++)
	{
		if (cDate.isDateWithinTwoDays(timeline[i]->getDate()))
		{
			timeline[i]->viewPost();
			cout << endl;
		}
	}
	for (int i = 0; i < friendsTotal; i++)
	{
		for (int j = 0; j < FriendList[i]->totalTimelinePosts; j++)
		{
			if (cDate.isDateWithinTwoDays(FriendList[i]->timeline[j]->getDate()))
			{
				FriendList[i]->timeline[j]->viewPost();
				cout << endl;
			}
		}
	}

	for (int i = 0; i < pagesTotal; i++)
	{
		for (int j = 0; j < LikedPages[i]->getTotalTimelinePosts(); j++)
		{
			if (cDate.isDateWithinTwoDays(LikedPages[i]->getTimeLinePost(j)->getDate()))
			{
				LikedPages[i]->getTimeLinePost(j)->viewPost();
				cout << endl;
			}
		}
	}
}
void User::SeeMemories()
{
	for (int i = 0; i < totalTimelinePosts; i++)
	{
		if (Date::currentDate.isMemory(timeline[i]->getDate()))
		{
			cout << Date::currentDate.getYear() - timeline[i]->getDate().getYear() << " Years Ago\n";
			timeline[i]->viewPost();
		}
	}
}
void User::shareMemory(Post * p, char* t)
{
	timeline[totalTimelinePosts] = new Memory(p, t);
}


//-------------PAGE FUNCTIONS--------------//
Page::Page()
{
	ID = new char[5];
	Title = new char[80];
}
void Page::ReadDataFromFile(ifstream & fin)
{
	fin >> ID;
	ID = Helper::shrinkarray(ID);
	fin.ignore();
	fin.getline(Title, 79);									//THIS IS HOW TO INPUT A LINE INSTEAD OF WORDS
	Title = Helper::shrinkarray(Title);
}
void Page::PrintInfo()
{
	cout << ID << " - " << Title << endl;
}
void Page::addToTimeline(Post * ptr)
{
	timeline[totalTimelinePosts] = ptr;
	totalTimelinePosts++;
}

//-------------POST FUNCTIONS--------------//
Post::Post()
{
	postID = new char[10];
	datePosted = Date();
	text = new char[100];
	sharedBy = 0;
	likedBy = new AppNetwork * [10];
	for (int i = 0; i < 10; i++)
		likedBy[i] = nullptr;
	likes = 0;
	totalComments = 0;
	comments = new Comment * [10];
	for (int i = 0; i < 10; i++)
	{
		comments[i] = nullptr;
	}
	content = new PostContent * ();
	content[0] = nullptr;
}
void Post::ReadDataFromFile(ifstream & fin)
{
	fin >> postID;
	postID = Helper::shrinkarray(postID);
	int d, m, y;
	fin >> d >> m >> y;
	datePosted.setValues(d, m, y);
	fin.ignore();
	fin.getline(text, 99);
	text = Helper::shrinkarray(text);
}
void Post::viewPost()
{
	cout << "--- ";
	sharedBy->printUserName();
	if (content[0])
		content[0]->print();
	cout << " (";
	datePosted.printDate();
	cout << ")";
	cout << endl << "\t\"";
	cout << text;
	cout << ".\"";
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		if (comments[i])
		{
			cout << "\t\t";
			comments[i]->Print();
		}
	}
}
void Post::printPost()
{
	cout << postID << " shared by ";
	sharedBy->PrintInfo();
}

//-------------MEMORY FUNCTIONS--------------//
void Memory::viewPost()
{
	cout << "---";
	oldPostPtr->getSharedBy()->printUserName();
	cout << " shared a memory---(";
	Date::currentDate.printDate();
	cout << ")\n\t";

	cout << getText() << endl;

	int yearsPassed = 0;
	yearsPassed = (Date::currentDate.getYear()) - (oldPostPtr->getDate().getYear());
	cout << "\t----" << yearsPassed << " Years Ago----\n";
	cout << "------------------------------------------------------------------\n";
	cout << "---";
	oldPostPtr->getSharedBy()->printUserName();
	if (oldPostPtr->getPostContent())
		oldPostPtr->getPostContent()->print();
	cout << "(";
	oldPostPtr->getDate().printDate();
	cout << ")\n\t";
	cout << oldPostPtr->getText() << endl;
}
Memory::Memory(Post * old, char* newText)
{
	oldPostPtr = old;
	setText(newText);
}

//-------------COMMENTS FUNCTIONS--------------//
Comment::Comment(char* cID, Post * cOn, AppNetwork * cBy, char* textOfComment)
{
	ID = new char[10];
	Helper::strcopy(cID, ID);
	ID = Helper::shrinkarray(ID);
	commentedOn = cOn;
	commentBy = cBy;
	text = new char[100];
	Helper::strcopy(textOfComment, text);
	text = Helper::shrinkarray(text);
}
void Comment::Print()
{
	commentBy->printUserName();
	cout << ": " << text << endl;
}

//-------------DATE FUNCTIONS--------------//
Date::Date()
{
	date = 1;
	month = 1;
	year = 1960;
}
void Date::setValues(int d, int m, int y)
{
	date = d;
	month = m;
	year = y;
}
void Date::printDate()
{
	cout << date << "/" << month << "/" << year;
}
bool Date::isDateWithinTwoDays(Date rhs)
{
	if ((date == rhs.date && month == rhs.month && year == rhs.year) || (date == rhs.date + 1 && month == rhs.month && year == rhs.year))
		return true;
	else
		return false;
}
bool Date::isMemory(Date rhs)
{
	if (date == rhs.date && month == rhs.month && year != rhs.year)
	{
		return true;
	}
	else
		return false;
}

//-------------POST CONTENT FUNCTIONS--------------//
PostActivity::PostActivity(int i, char* t)
{
	text = new char[80];
	type = i;
	Helper::strcopy(t, text);
	text = Helper::shrinkarray(text);
}
void PostActivity::print()
{
	if (type == 1)
		cout << " is feeling";
	else if (type == 2)
		cout << " is thinking about";
	else if (type == 3)
		cout << " is making";
	else if (type == 4)
		cout << " is celebrating";
	cout << text;
}

//-------------CONTROLLER FUNCTIONS--------------//
void Controller::LoadAllUser(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file Social Network Users.\n";
	}

	int usersTot = 0;
	fin >> usersTot;
	User::setTotalUser(usersTot);
	AllUsers = new User * [usersTot];
	for (int i = 0; i < usersTot; i++)
	{													//??? MEANS NO MEMORY ALLOCATION, XCDCDCDCDC MEANS DEFINED, AOGDVUSDPF MEAND ALLOCATED
		AllUsers[i] = new User[1];						//THIS SPACE ALLOCATION IS NECESSARY, POINTER DOESNT CALL CONSTRUCTOR, THE ACTUAL NEW USER DOES AND ALLOCATES SPACE
		AllUsers[i]->ReadDataFromFile(fin);				//ASK:WILL CONSTRUCTOR BE CALLED HERE? DO WE NEED TO ALLOCATE AGAIN?
	}
	fin.close();

}
void Controller::LoadAllPages(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file Social Network Users.\n";
	}

	int pagesTot = 0;
	fin >> pagesTot;
	Page::setPageTotal(pagesTot);
	AllPages = new Page * [pagesTot];
	for (int i = 0; i < pagesTot; i++)
	{
		AllPages[i] = new Page[1];
		AllPages[i]->ReadDataFromFile(fin);
	}
	fin.close();
}
void Controller::LoadAllPosts(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file Social Network Posts.\n";
		return;
	}

	int totPosts;
	fin >> totPosts;
	Post::setTotalPosts(totPosts);

	AppNetwork* tempNetwork = 0;
	char tempID[5], end[3] = "-1";

	AllPosts = new Post * [totPosts];
	for (int i = 0; i < totPosts; i++)
	{
		AllPosts[i] = new Post[1];
		AllPosts[i]->ReadDataFromFile(fin);
		fin >> tempID;
		if (tempID[0] == 'u')
			tempNetwork = SearchUserByID(tempID);
		else if (tempID[0] == 'p')
			tempNetwork = SearchPageByID(tempID);
		AllPosts[i]->setSharedBy(tempNetwork);
		tempNetwork->addToTimeline(AllPosts[i]);
		fin >> tempID;
		while (!Helper::compareText(end, tempID))
		{
			if (tempID[0] == 'u')
				tempNetwork = SearchUserByID(tempID);
			else if (tempID[0] == 'p')
				tempNetwork = SearchPageByID(tempID);
			AllPosts[i]->setLikedBy(tempNetwork);
			//Add to timeline here the post as well
			fin >> tempID;
		}
		fin.ignore();
	}
	fin.close();
}
void Controller::LoadAllComments(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file Social Network Comments.\n";
		return;
	}

	int totComments;
	fin >> totComments;
	Comment::setTotalComments(totComments);

	char cID[10], text[80], bufferID[10];
	Post* commentOn;
	AppNetwork* commentBy;

	for (int i = 0; i < totComments; i++)
	{
		fin >> cID;
		fin >> bufferID;
		commentOn = SearchPostByID(bufferID);
		fin >> bufferID;
		commentBy = SearchNetworkMemberByID(bufferID);
		fin.ignore();
		fin.getline(text, 79);
		Comment* comment = new Comment(cID, commentOn, commentBy, text);
		commentOn->setComments(comment);
		commentOn->incrementComments();
	}
}
void Controller::LinkUsersAndPages(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file linking pages and users.\n";
		return;
	}

	char buffer[5], end[3];
	end[0] = '-';
	end[1] = '1';
	end[2] = '\0';
	User* currentUser;
	User* addedUser;
	Page* likedPage;
	fin >> buffer;
	while (!Helper::compareText(buffer, end))
	{
		currentUser = SearchUserByID(buffer);
		fin >> buffer;
		while (!Helper::compareText(buffer, end))
		{
			addedUser = SearchUserByID(buffer);
			currentUser->AddFriend(addedUser);
			fin >> buffer;
		}
		fin >> buffer;
		while (!Helper::compareText(buffer, end))
		{
			likedPage = SearchPageByID(buffer);
			currentUser->LikePage(likedPage);
			fin >> buffer;
		}
		fin >> buffer;
	}

}
void Controller::LoadAllActivities(const char* file)
{
	int totalActivities, bufferType;
	char bufferid[10], bufferText[80];
	Post* activityOn;

	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "Error opening the file Social Network Activities.\n";
		return;
	}

	fin >> totalActivities;
	for (int i = 0; i < totalActivities; i++)
	{
		fin >> bufferid;
		activityOn = SearchPostByID(bufferid);
		fin >> bufferType;
		fin.getline(bufferText, 79);
		PostActivity* activityBuffer = new PostActivity(bufferType, bufferText);
		activityOn->setPostContent(activityBuffer);
	}

}
AppNetwork* Controller::SearchNetworkMemberByID(char* id)
{
	switch (id[0]) {

	case'u':
	case'U':
		return SearchUserByID(id);
	case 'p':
	case 'P':
		return SearchPageByID(id);
	default:
		return nullptr;
	}

}
User* Controller::SearchUserByID(char* id)
{
	int usersTot = User::getTotalUsers();
	for (int i = 0; i < usersTot; i++)
	{
		if (Helper::compareText(AllUsers[i]->getID(), id))
		{
			//cout << "User found: ";
			//AllUsers[i]->PrintUser();
			return AllUsers[i];
		}
	}
}
Page* Controller::SearchPageByID(char* id)
{
	int pagesTot = Page::getPageTotal();
	for (int i = 0; i < pagesTot; i++)
	{
		if (Helper::compareText(AllPages[i]->getID(), id))
		{
			return AllPages[i];
		}
	}
}
Post* Controller::SearchPostByID(char* id)
{
	int postsTot = Post::getPostTotal();
	for (int i = 0; i < postsTot; i++)
	{
		if (Helper::compareText(AllPosts[i]->getID(), id))
		{
			return AllPosts[i];
		}
	}
}

//-------------DESTRUCTORS FUNCTIONS--------------//

User::~User()
{
	if (ID)
		delete[] ID;
	if (Fname)
		delete[] Fname;
	if (Lname)
		delete[] Lname;
	if (FriendList)
		delete[]  FriendList;
	if (LikedPages)
		delete[] LikedPages;
}
AppNetwork::~AppNetwork()
{
	for (int i = 0; i < totalTimelinePosts; i++)
		if (timeline[i])
			delete[] timeline[i];
	if (timeline)
		delete[] timeline;
}
Post::~Post()
{
	//cout << "Post destrucote called for " << getID() << endl;
	if (postID)
		delete[] postID;
	if (text)
		delete[] text;
	if (likedBy)
		delete[] likedBy;
	for (int i = 0; i < totalComments; i++)
	{
		delete[] comments[i]->getID();
		delete[] comments[i]->getText();
		delete[] comments[i];
	}
	delete[] comments;
	if (content[0])
		delete[] content[0];
	delete[] content;
}
Page::~Page()
{
	/*cout << "Post Destructor: Timeline deleted for ";
	this->printUserName();*/
	//cout << endl;
	if (ID)
		delete[] ID;
	if (Title)
		delete[] Title;
}
PostActivity::~PostActivity()
{
	//cout << "PostActivity Destructor: ";
	//cout << endl;
	if (text)
		delete[] text;
}
PostContent::~PostContent()
{
	//cout << "PostContent Destructor: ";
}
Controller::~Controller()
{
	for (int i = 0; i < 20; ++i)
	{
		if (AllUsers[i])
			delete[] AllUsers[i];
	}
	delete[] AllUsers;

	// Delete AllPages if it's allocated
	if (AllPages)
	{
		for (int i = 0; i < 12; ++i)
			if (AllPages[i])
				delete[] AllPages[i];
		delete[] AllPages;
	}
	if (AllPosts)
	{
		delete[] AllPosts;
	}
}
Memory::~Memory()
{
	oldPostPtr = 0;
}
Date::~Date()
{
	//cout << "Date Destructor called\n";
}

//Why is comment destructor giving me error???