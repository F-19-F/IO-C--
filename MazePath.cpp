#include <iostream>
#include <stack>
using namespace std;
/**
 * 
 * 
 *               一个用栈实现的自动走迷宫 
 * 
 * 
 *                  Author:F19
 * 
 * 
 **/
class Pos
{
public:
    int x;
    int y;
    bool operator==(Pos &t);
};
bool Pos::operator==(Pos &t)
{
    if (t.x==x&&t.y==y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
class MazeMap
{
private:
    bool *base;
public:
    int r,c;
    MazeMap(int x,int y);
    bool Pass(Pos t);
    bool Edit(Pos t,bool value);
    void ShowMap();
    ~MazeMap();
};
MazeMap::MazeMap(int r,int c)
{
    base=new bool[r*c]();//new并＋()可以分配并初始化
    this->r=r;
    this->c=c;
}
MazeMap::~MazeMap()
{
    delete [] base;
}
void MazeMap::ShowMap()
{
    for (int i = 0; i <r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (*(base + (r - 1 - i) *c + j))
            {
                cout<<"X";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"Y=------ "<<r - i;
        cout<<endl;
    }
    for (int i = 0; i <c; i++)
    {
        cout<< i + 1;
    }
    cout<<"-----------=X";
    cout<<endl;
}
bool MazeMap::Pass(Pos t)
{
    if (t.x<=0||t.y<=0||t.x>c||t.y>r)
    {
        return false;
    }
    if (!*(base+(t.y-1)*c+(t.x-1)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool MazeMap::Edit(Pos t,bool value)
{
    if (t.x>c||t.y>r||t.x<=0||t.y<=0)
    {
        return false;
    }
    *(base+(t.y-1)*c+(t.x-1))=value;
    return true;
}
class MazeBase
{
public:
    Pos locate;
    int Try_step;
    MazeBase(Pos locat);
    MazeBase();
    MazeBase NextStep();
};
MazeBase::MazeBase()
{
    
}
MazeBase::MazeBase(Pos locat)
{
    this->locate=locat;
    Try_step=0;
}
/*坐标系为数学上常见的坐标系*/
//1,2,3,4分别代表向西，南，东，北
MazeBase MazeBase::NextStep()
{
    Pos New=locate;
    switch (++Try_step)
    {
    case 1:
        New.x--;
        break;
    case 2:
        New.y--;
        break;
    case 3:
        New.x++;
        break;
    case 4:
        New.y++;
        break;
    default:
        break;
    }
    MazeBase result(New);
    return result;
}
stack <MazeBase> K;
bool MazePath(Pos start,Pos end,MazeMap &map)
{
    MazeMap Black_list(map.r,map.c);
    MazeBase temp(start);
    MazeBase New_Pos=temp.NextStep();
    do
    {
        //如果新位置通过，并且没有回到原来的位置
        if (map.Pass(New_Pos.locate)&&Black_list.Pass(New_Pos.locate))
        {
            //先将前面新位置的前面一个元素压栈
            K.push(temp);
            Black_list.Edit(temp.locate,true);
            //如果新位置是出口，则将新位置压栈并退出
            if (New_Pos.locate==end)
            {
                K.push(New_Pos);
                return true;
            }
            else//不是出口则更新当前位置并在新的当前位置基础上寻找新位置
            {
                temp=New_Pos;
                New_Pos=temp.NextStep();
            }
        }
        else//如果新位置不通过
        {
            //如果当前位置还有路没有试，则尝试新位置
            if (temp.Try_step!=4)
            {
                New_Pos=temp.NextStep();
            }
            else//没有路可以试
            {
                //将所有没有路可走的位置出栈
                while (temp.Try_step==4&&!K.empty())
                {
                    temp=K.top();
                    K.pop();
                }
                //栈已经空了
                if(temp.Try_step==4)
                {
                    return false;
                }
                else//在栈中找到了还能走的位置，在该位置的基础上寻找新位置
                {
                    New_Pos=temp.NextStep();
                }
            }
            
        }
        //当栈非空或者temp还有路可以走，则一直循环
    } while (!K.empty()||temp.Try_step!=4);
    return false;
}
int main()
{
    Pos INIT;
    Pos Start,End;
    MazeBase Temp;
    int x,y;
    char t;
    cout<<"What's the size of map?:(r,c)";
    cin>>x>>y;
    cout<<"Draw your map:"<<endl;
    MazeMap map(x,y);
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            INIT.x = j + 1;
            INIT.y = x - i;
            scanf("%c", &t);
            if (t == '\n')
            {
                scanf("%c", &t);
            }
            if (t == ' ')
            {

                if(!map.Edit(INIT, false))
                {
                    cout<<"Error!!"<<endl;
                }
            }
            else
            {
                if(!map.Edit(INIT, true))
                {
                    cout<<"Error!!!!"<<endl;
                }
            }
        }
        getchar();
    }
    cout<<"Your Map:"<<endl;
    map.ShowMap();
    cout<<"From?:";
    cin>>x>>y;
    Start.x=x;
    Start.y=y;
    cout<<"To:?";
    cin>>x>>y;
    End.x=x;
    End.y=y;
    if(MazePath(Start,End,map))
    {
        cout<<"Your Path:"<<endl;
        while (!K.empty())
        {
            Temp=K.top();
            K.pop();
            cout<<"("<<Temp.locate.x<<","<<Temp.locate.y<<")";
            switch (Temp.Try_step)
            {
            case 1:
                cout<<"Left"<<endl;
                break;
            case 2:
                cout<<"Down"<<endl;
                break;
            case 3:
                cout<<"Rigth"<<endl;
                break;
            case 4:
                cout<<"Up"<<endl;
                break;
            default:
                cout<<"OK!"<<endl;
                break;
            }
        }
    }
    else
    {
        cout<<"No Path found!";
    }
    return 0;
}