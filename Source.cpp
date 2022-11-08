#include<bits/stdc++.h>
#define Course_File_NAME "courses.txt"
using namespace std;
class BinaryCourseTree
{
public: int // declaration of tree class and two members
    CourseCode;
      BinaryCourseTree* Child_left;
      BinaryCourseTree* Child_right;
      BinaryCourseTree(int CourseCode) // constructor for the tree class takes an int parameter
      {
          this->CourseCode = CourseCode;
          Child_left = Child_right = NULL;// declared as null to indicate it doesnt exist yet
      }
};
BinaryCourseTree*
buildTree(map<string, vector<string>>& coursePairs, string rootCourse)
{
    int CourseCode;
    stringstream ss(rootCourse);
    ss >> CourseCode;
    BinaryCourseTree* root = new BinaryCourseTree(CourseCode);
    if (coursePairs[rootCourse].size() > 0) root->Child_left = buildTree(coursePairs, coursePairs[rootCourse][0]);
    if (coursePairs[rootCourse].size() > 1) root->Child_right = buildTree(coursePairs, coursePairs[rootCourse][1]);
    return root;
}
BinaryCourseTree* fetchPreRequisites(map<string, vector<string>>& coursePairs)
{
    fstream fin;
    fin.open(Course_File_NAME, ios::in);
    string courses,
        rootCourse;
    while (getline(fin, courses))
    {
        stringstream c(courses);
        string course, Parent_Course, Child_Course;
        bool parent = false;
        while (getline(c, course, ','))
        {
            if (parent) Parent_Course = course;
            else {
                Child_Course = course;
                parent = true;
            }
        }
        if (Parent_Course == "None")
            rootCourse = Child_Course;
        else coursePairs[Parent_Course].push_back(Child_Course);
    }
    return buildTree(coursePairs, rootCourse);
}
void readInput(vector<string>& coursesAlreadyStudied, vector<string>& NewCourses)
{
    cout << "Enter the courses which you already studied & Enter EXIT to stop the input:\n";
    string courseName;
    while (cin >> courseName)
    {
        if (courseName == "EXIT") break;
        coursesAlreadyStudied.push_back(courseName);
    }
    cout << "Enter the courses which you want to study & Enter EXIT to stop the input:\n";
    while (cin >> courseName)
    {
        if (courseName == "EXIT")
            break;
        NewCourses.push_back(courseName);
    }
}
bool
searchCourse(BinaryCourseTree* root, int courseCode, vector<int>& StudiedCourseCodes)
{
    if (root == NULL) return false;
    if (root->CourseCode == courseCode)
        return true;
    if (searchCourse(root->Child_left, courseCode, StudiedCourseCodes))
        return
        count(StudiedCourseCodes.begin(), StudiedCourseCodes.end(), root->CourseCode);
    if (searchCourse(root->Child_right, courseCode, StudiedCourseCodes))
        return count(StudiedCourseCodes.begin(), StudiedCourseCodes.end(), root->CourseCode);
    return false;
}
void fetchResults(BinaryCourseTree* root, vector<string>& coursesAlreadyStudied, vector<string>& NewCourses)
{
    vector<int> StudiedCourseCodes;
    for (string course : coursesAlreadyStudied)
    {
        stringstream ss(course);
        int courseCode;
        ss >> courseCode;
        StudiedCourseCodes.push_back(courseCode);
    }
    vector<string> canStudy;
    vector<string> cannotStudy;
    for (string course : NewCourses)
    {
        bool courseFound = false;
        stringstream ss(course);
        int courseCode;
        ss >> courseCode;
        if (searchCourse(root, courseCode, StudiedCourseCodes))
            canStudy.push_back(course);
        else
            cannotStudy.push_back(course);
    }
    cout << "\n the following courses can be taken:\n";
    for (string course : canStudy)
        cout << course << endl; cout << "\nThe following courses are CANNOT be taken\n";
    for (string course : cannotStudy)
        cout << course << endl;
}
int main()
{
    map<string, vector<string>> coursePairs;
    BinaryCourseTree* root = fetchPreRequisites(coursePairs);
    vector<string> coursesAlreadyStudied;
    vector<string> NewCourses;
    readInput(coursesAlreadyStudied, NewCourses);
    cout << "\nProcessing.....\n";
    fetchResults(root, coursesAlreadyStudied, NewCourses);
    return 0;
}