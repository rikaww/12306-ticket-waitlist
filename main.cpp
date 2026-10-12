#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
#include <limits>

using namespace std;

// 候补订单信息结构体
struct WaitlistOrder {
    string orderId;       // 订单号
    string passengerName; // 旅客姓名
    string idCard;        // 身份证号
    int ticketCount;      // 所需票数
};

// 工具函数：按回车暂停，防止屏幕瞬间刷新
void pauseConsole() {
    cout << "\n--------------------------------------------------" << endl;
    cout << ">>> 请按【回车键(Enter)】返回主菜单...";
    // 清空输入流并等待用户按下回车
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

class TicketWaitlistSystem {
private:
    queue<WaitlistOrder> waitlistQueue; // 候补等待队列 (先进先出 FIFO)
    int orderCounter = 1001;            // 自动编号

public:
    // 1. 新增候补订单：将订单加入队列尾部（入队）
    void addOrder() {
        WaitlistOrder order;
        order.orderId = "HB" + to_string(orderCounter++);
        
        cout << "\n==================================================" << endl;
        cout << "            【操作 1：新增候补订单（入队）】        " << endl;
        cout << "==================================================" << endl;
        cout << "系统已分配订单号: " << order.orderId << endl;
        
        cout << "请输入旅客姓名: ";
        cin >> order.passengerName;

        cout << "请输入身份证号: ";
        cin >> order.idCard;

        cout << "请输入所需票数 (默认请输入 1): ";
        cin >> order.ticketCount;

        if (order.ticketCount <= 0) {
            order.ticketCount = 1;
        }

        // 入队
        waitlistQueue.push(order);

        cout << "\n--------------------------------------------------" << endl;
        cout << "【成功反馈】：候补订单已成功加入队列尾部！" << endl;
        cout << "  - 订单编号: " << order.orderId << endl;
        cout << "  - 旅客姓名: " << order.passengerName << endl;
        cout << "  - 当前排队位置: 第 " << waitlistQueue.size() << " 位 (队尾)" << endl;
        cout << "--------------------------------------------------" << endl;

        pauseConsole();
    }

    // 2. 余票发放：输入本次释放余票数量k，从队头依次处理
    void releaseTickets() {
        cout << "\n==================================================" << endl;
        cout << "            【操作 2：余票发放与出队兑现】        " << endl;
        cout << "==================================================" << endl;

        // 如果一开始就是空的
        if (waitlistQueue.empty()) {
            cout << "\n【系统提示】：当前候补队列为空！没有订单需要兑现。" << endl;
            cout << "【状态说明】：释放的余票将转入公共票池，候补队伍无需处理。" << endl;
            pauseConsole();
            return;
        }

        int k;
        cout << "请输入本次释放的余票数量 (k): ";
        if (!(cin >> k) || k <= 0) {
            cout << "\n【输入错误】：释放余票数量必须是大于 0 的整数！" << endl;
            pauseConsole();
            return;
        }

        int originalK = k;
        int successCount = 0;

        cout << "\n>>> 开始从【队头】依次为旅客出票兑现：" << endl;
        cout << "--------------------------------------------------" << endl;

        while (!waitlistQueue.empty() && k > 0) {
            WaitlistOrder frontOrder = waitlistQueue.front();

            if (k >= frontOrder.ticketCount) {
                // 扣减余票
                k -= frontOrder.ticketCount;
                // 订单候补成功，出队
                waitlistQueue.pop();
                successCount++;

                cout << "[出票成功] 订单号: " << frontOrder.orderId 
                     << " | 姓名: " << frontOrder.passengerName 
                     << " | 身份证: " << frontOrder.idCard 
                     << " | 获得车票: " << frontOrder.ticketCount << " 张" << endl;
            } else {
                cout << "[票额不足] 队首订单 " << frontOrder.orderId 
                     << " (" << frontOrder.passengerName << ") 需要 " 
                     << frontOrder.ticketCount << " 张票，当前仅剩 " << k 
                     << " 张，票额不足以满足整单，继续在队首等待。" << endl;
                break;
            }
        }

        cout << "--------------------------------------------------" << endl;
        cout << "【本次放票结算】：释放 " << originalK << " 张票，成功兑现 " 
             << successCount << " 单，剩余票额: " << k << " 张。" << endl;

        // 如果本次放完后余票为 0
        if (k == 0) {
            if (waitlistQueue.empty()) {
                cout << "【队列状态】：所有候补已全部处理完毕，候补队列已空！" << endl;
            } else {
                cout << "【队列状态】：余票已耗尽，队列中仍有 " << waitlistQueue.size() 
                     << " 个订单继续在队中等待下次放票。" << endl;
            }
        }

        pauseConsole();
    }

    // 3. 余票为0时，判断候补队列是否为空；如不为空继续等待，新增订单入队尾
    void checkWaitlistWhenNoTickets() {
        cout << "\n==================================================" << endl;
        cout << "      【操作 3：余票为0时，判断候补队列状态】      " << endl;
        cout << "==================================================" << endl;

        // 显式判断队列是否为空
        if (waitlistQueue.empty()) {
            cout << "\n【系统提示】：当前余票为 0，候补队列【为空】！" << endl;
            cout << "【排队情况】：目前没有任何旅客在候补排队。" << endl;
            cout << "【系统建议】：如需购票，请在主菜单选择 1 提交新订单加入队尾。" << endl;
        } else {
            cout << "\n【系统提示】：当前余票为 0！" << endl;
            cout << "【判断结果】：候补队列【不为空】！" << endl;
            cout << "【处理动作】：共有 " << waitlistQueue.size() 
                 << " 个订单未兑现，全部【继续在队中等待】下次放票！" << endl;
            
            cout << "\n--- 当前队列排队明细（按队头到队尾先后排序）---" << endl;
            queue<WaitlistOrder> tempQ = waitlistQueue;
            int rank = 1;
            while (!tempQ.empty()) {
                WaitlistOrder o = tempQ.front();
                tempQ.pop();
                cout << "第 " << rank++ << " 位 (队" << (rank == 2 ? "头" : (tempQ.empty() ? "尾" : "中")) << ")"
                     << " | 单号: " << o.orderId 
                     << " | 旅客: " << left << setw(8) << o.passengerName 
                     << " | 身份证: " << o.idCard 
                     << " | 需求: " << o.ticketCount << "张" << endl;
            }
            cout << "--------------------------------------------------" << endl;
            cout << "根据规则：余票为0且队伍不空时，新增订单将继续加入队尾。" << endl;
        }

        pauseConsole();
    }
};

int main() {
    TicketWaitlistSystem sys;
    int choice;

    while (true) {
        cout << "\n**************************************************" << endl;
        cout << "         模拟 12306 车票候补系统                     " << endl;
        cout << "**************************************************" << endl;
        cout << "  1. 新增候补订单 (入队)" << endl;
        cout << "  2. 余票发放与出队兑现 (出队)" << endl;
        cout << "  3. 余票为0时的候补状态检查 (判断队列是否为空)" << endl;
        cout << "  4. 退出程序" << endl;
        cout << "**************************************************" << endl;
        cout << "请输入数字选择功能 [1-4]: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n【警告】输入无效，请输入 1 到 4 的数字！" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                sys.addOrder();
                break;
            case 2:
                sys.releaseTickets();
                break;
            case 3:
                sys.checkWaitlistWhenNoTickets();
                break;
            case 4:
                cout << "\n==================================================" << endl;
                cout << ">>> 已退出 12306 车票候补模拟系统。演示结束！" << endl;
                cout << "==================================================" << endl;
                return 0;
            default:
                cout << "\n【警告】选项不存在，请输入 1-4 之间的选项！" << endl;
        }
    }

    return 0;
}
