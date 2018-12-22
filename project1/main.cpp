#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>


using namespace std;

struct factory{
    int cost;
    int capacity;
    int cnt;
    factory(int c1, int c2, int c3):cost(c1), capacity(c2), cnt(c3){
    }
};

struct customer
{
    int fac;
    int demand;
    vector<int> costs;
};

vector<factory> factorys;
vector<customer> customers;
vector<factory> bfactorys;
vector<customer> bcustomers;

int SA_search()
{
    int i, j, cnt = 0, cost = 0, pastcost = 0, bestcost = 1000000, past = 0;
    int lcost = 0;
    float ncnt = 0;
    float t;
    for(i=0;i<customers.size();i++)
    {
        int ran = rand()%factorys.size();
        if(factorys[ran].capacity - factorys[ran].cnt < customers[i].demand)
        {
            i--;
            /*cnt++;
            if(cnt>10000)
                i = -1;*/
            continue;
        }
        factorys[ran].cnt += customers[i].demand;
        customers[i].fac = ran;
        cost += (customers[i].costs)[ran];
    }
    for(i=0;i<factorys.size();i++)
    {
        if(factorys[i].cnt!=0)
            cost += factorys[i].cost;
    }
    cout<<cost<<endl;
    t = 1500;
    cnt = 0;
    for(i=0;i<3000;i++)
    {
        for(j=0;j< customers.size() * factorys.size() / 10;j++)
        {
            int ran = rand()%10;
            if(ran<5)
            {
                int changedCus = rand()%customers.size();
                int changedFac = rand()%factorys.size();
                if(changedFac == customers[changedCus].fac)
                    continue;
                if(factorys[changedFac].capacity - factorys[changedFac].cnt < customers[changedCus].demand)
                    continue;
                customer cc = customers[changedCus];
                //past = cc.fac;
                int changedCost = - (cc.costs)[cc.fac] + (cc.costs)[changedFac];
                if(factorys[changedFac].cnt==0)
                    changedCost += factorys[changedFac].cost;
                if(factorys[cc.fac].cnt == cc.demand)
                    changedCost -= factorys[cc.fac].cost;
                int r1 = rand() % 32767;
                float r2 = r1 / 32767.0;

                if(exp(-changedCost/t)> r2)
                {
                    factorys[cc.fac].cnt -=cc.demand;
                    customers[changedCus].fac = changedFac;
                    factorys[changedFac].cnt +=cc.demand;
                    cost += changedCost;
                }
            }
            else
            {
                int changedCus1 = rand()% customers.size();
                int changedCus2 = rand()% customers.size();
                customer cc1 = customers[changedCus1];
                customer cc2 = customers[changedCus2];
                if(cc1.demand>cc2.demand)
                {
                    if(factorys[cc2.fac].capacity-factorys[cc2.fac].cnt<cc1.demand-cc2.demand)
                        continue;
                }
                else
                {
                    if(factorys[cc1.fac].capacity-factorys[cc1.fac].cnt<cc2.demand-cc1.demand)
                        continue;
                }
                int changedCost = (cc1.costs)[cc2.fac] + (cc2.costs)[cc1.fac];
                changedCost -= ((cc1.costs)[cc1.fac] + (cc2.costs)[cc2.fac]);
                int r1 = rand() % 32767;
                float r2 = r1  / 32767.0;
                if(exp(-changedCost/t)> r2)
                {
                    factorys[cc1.fac].cnt -= cc1.demand;
                    factorys[cc1.fac].cnt += cc2.demand;
                    factorys[cc2.fac].cnt -= cc2.demand;
                    factorys[cc2.fac].cnt += cc1.demand;
                    int tmp = cc1.fac;
                    customers[changedCus1].fac = cc2.fac;
                    customers[changedCus2].fac = tmp;
                    cost += changedCost;
                }
            }
            lcost = 0;
        }

        t*=0.97;
        if(i%5==0)
        {
            if(cost==pastcost)
                cnt+=1;
            else
                cnt=0;
            if(cnt>=6)
            {
                t *= ncnt;
                ncnt++;
            }
            pastcost = cost;
            //cout<<i<<" cost: "<<cost<<" t: "<<t<<" ncnt:"<<ncnt<<endl;
        }
        if(cost<bestcost)
        {
            bfactorys = factorys;
            bcustomers = customers;
            bestcost = cost;
            ncnt = 3;
        }
    }
    lcost = 0;
    for(i=0;i<bcustomers.size();i++)
    {
        lcost += (bcustomers[i].costs)[bcustomers[i].fac];
    }

    for(i=0;i<bfactorys.size();i++)
    {
        if(bfactorys[i].cnt!=0)
            lcost += bfactorys[i].cost;
    }
    //cout<<lcost<<endl;
    return bestcost;
}

int Tabu_search()
{
    int tabulength = 10 + factorys.size();
    vector<int> tabuList;
    vector<int> tabuList2;
    int i, j, k, q, cnt = 0, cost = 0, bestcost = 1000000, mincost = 0,tfront = 0, minj = 0, mink = 0;
    for(i=0;i<customers.size();i++)
    {
        int ran = rand()%factorys.size();
        if(factorys[ran].capacity - factorys[ran].cnt < customers[i].demand)
        {
            i--;
            cnt++;
            if(cnt>10000)
                i = -1;
            continue;
        }
        factorys[ran].cnt += customers[i].demand;
        customers[i].fac = ran;
        cost += (customers[i].costs)[ran];
    }
    for(i=0;i<factorys.size();i++)
    {
        if(factorys[i].cnt!=0)
            cost += factorys[i].cost;
    }
    cout<<cost<<endl;
    for(i=0;i<1000;i++)
    {
        mincost = 1000000;
        for(j=0;j<customers.size();j++)
        {
            for(k=0;k<factorys.size();k++)
            {
                int changedCus = j;
                int changedFac = k;
                if(changedFac == customers[changedCus].fac)
                    continue;
                if(factorys[changedFac].capacity - factorys[changedFac].cnt < customers[changedCus].demand)
                    continue;
                customer cc = customers[changedCus];
                int changedCost = - (cc.costs)[cc.fac] + (cc.costs)[changedFac];
                if(factorys[changedFac].cnt==0)
                    changedCost += factorys[changedFac].cost;
                if(factorys[cc.fac].cnt == cc.demand)
                    changedCost -= factorys[cc.fac].cost;
                if(cost+changedCost<bestcost)
                {
                    minj = j;
                    mink = k;
                    mincost = changedCost;
                }
                for(q=tfront;q<tabuList.size();q++)
                {
                    if(j!=tabuList[q])
                        continue;
                    if(k!=tabuList2[q])
                        continue;
                    break;
                }
                if(q!=tabuList.size())
                {
                    //cout<<i<<" "<<q<<" "<<tabuList.size();
                    continue;
                }
                if(changedCost < mincost)
                {
                    minj = j;
                    mink = k;
                    mincost = changedCost;
                }
            }
        }
        //if(mincost==1000000)
            //cout<<i<<endl;
        factorys[customers[minj].fac].cnt -=customers[minj].demand;
        customers[minj].fac = mink;
        factorys[mink].cnt +=customers[minj].demand;
        cost += mincost;
        tabuList.push_back(minj);
        tabuList2.push_back(mink);
        if(tabuList.size()-tfront>tabulength)
            tfront++;
        if(cost<bestcost)
        {
            bfactorys = factorys;
            bcustomers = customers;
            bestcost = cost;

        }
        /*if(i%100==0)
        {
            cout<<i<<" cost: "<<cost<<endl;
        }*/
    }
    return bestcost;
}


int main()
{
    int i, j, k, fac, cust, cost;
    ofstream outFile;
    //outFile.open("../data.csv",ios::out);
    for(i=0;i<10;i++)
    {
        factorys.clear();
        customers.clear();
        stringstream ss;
        ss << (i + 1);
        string path = "../Instances/p" + ss.str();
        cout<<path<<endl;
        ifstream infile(path);
        path = "p" + ss.str();
        outFile << path <<",";

        infile>>fac>>cust;
        cout<<fac<<" "<<cust<<endl;
        /*cin>>t;
        cout<<t;*/
        for(j=0;j<fac;j++)
        {
            int tcost, tcap;
            infile>>tcap>>tcost;
            factory temp(tcost, tcap, 0);
            factorys.push_back(temp);
        }
        for(j=0;j<cust;j++)
        {
            float tdemand;
            infile>>tdemand;
            customer temp;
            temp.fac = -1;
            temp.demand = int(tdemand);
            customers.push_back(temp);
        }
        for(j=0;j<fac;j++)
        {
            for(k=0;k<cust;k++)
            {
                float tcost;
                infile>>tcost;
                customers[k].costs.push_back(int(tcost));
            }
        }
        vector<customer> customers2 = customers;
        vector<factory> factory2 = factorys;
        /*for(j=0;j<cust;j++)
        {
            cout<<customers[j].fac<<" "<<customers[j].demand<<endl;
            for(k=0;k<fac;k++)
            {
                cout<<customers[j].costs[k]<<endl;
            }
            cout<<endl;
        }*/
        clock_t start1,end1;
        start1 = clock();
        cost = SA_search();
        cout<<cost<<endl;
        end1 = clock();
        double dur = (double)(end1 - start1);
        cout<<"time: "<<(dur/CLOCKS_PER_SEC)<<endl;
        outFile<<cost<<","<<(dur/CLOCKS_PER_SEC)<<",";
        for(j=0;j<factorys.size();j++)
        {
            if(factorys[j].cnt>0)
                cout<<1;
            else
                cout<<0;
            cout<<" ";
        }
        cout<<endl;
        for(j=0;j<customers.size();j++)
        {
            cout<<customers[j].fac<<" ";
        }
        cout<<endl;

        factorys = factory2;
        customers = customers2;
        start1 = clock();
        cost = Tabu_search();

        cout<<cost<<endl;
        end1 = clock();
        dur = (double)(end1 - start1);
        cout<<"time: "<<(dur/CLOCKS_PER_SEC)<<endl;
        outFile<<cost<<","<<(dur/CLOCKS_PER_SEC)<<endl;
        for(j=0;j<factorys.size();j++)
        {
            if(factorys[j].cnt>0)
                cout<<1;
            else
                cout<<0;
            cout<<" ";
        }
        cout<<endl;
        for(j=0;j<customers.size();j++)
        {
            cout<<customers[j].fac<<" ";
        }
        cout<<endl;
        infile.close();
    }
    outFile.close();
    return 0;
}
