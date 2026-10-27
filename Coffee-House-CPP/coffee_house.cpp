#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Product { int id{}; string name, category; double price{}; int stock{}; bool available{true}; };
struct OrderItem { int productId{}; string productName; int quantity{}; double unitPrice{}; };
struct Order {
    int id{}; vector<OrderItem> items; string customerName, paymentMethod, status{"PLACED"};
    double subtotal{}, discount{}, total{};
};

class CoffeeShop {
    vector<Product> products;
    vector<Order> orders;
    int nextOrderId = 1001;

    static void pause() { cout << "\nPress Enter to continue..."; cin.ignore(numeric_limits<streamsize>::max(),'\n'); cin.get(); }
    static int readInt(const string& p,int min=0) {
        while(true){ cout<<p; int v; if(cin>>v&&v>=min){cin.ignore(numeric_limits<streamsize>::max(),'\n');return v;}
        cout<<"Invalid input.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); }
    }
    static double readDouble(const string& p,double min=0) {
        while(true){ cout<<p; double v; if(cin>>v&&v>=min){cin.ignore(numeric_limits<streamsize>::max(),'\n');return v;}
        cout<<"Invalid amount.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); }
    }
    static string readLine(const string& p){ cout<<p; string s; getline(cin,s); return s; }

    Product* findProduct(int id){ for(auto& p:products) if(p.id==id) return &p; return nullptr; }
    void seedProducts(){
        products={{101,"Espresso","Coffee",90,30,true},{102,"Cappuccino","Coffee",140,25,true},
        {103,"Cafe Latte","Coffee",150,25,true},{104,"Cold Coffee","Beverage",160,20,true},
        {105,"Masala Tea","Tea",80,35,true},{106,"Veg Sandwich","Snacks",130,18,true},
        {107,"Chicken Sandwich","Snacks",180,15,true},{108,"Chocolate Cake","Dessert",120,12,true}};
    }
    void header(const string& t) const { cout<<"\n============================================================\n"<<setw(38)<<t<<"\n============================================================\n"; }

    void showMenu() const {
        cout<<left<<setw(6)<<"ID"<<setw(22)<<"Product"<<setw(15)<<"Category"<<right<<setw(10)<<"Price"<<setw(10)<<"Stock"<<"\n";
        cout<<string(63,'-')<<"\n";
        for(const auto& p:products) if(p.available)
            cout<<left<<setw(6)<<p.id<<setw(22)<<p.name<<setw(15)<<p.category<<right<<setw(10)<<fixed<<setprecision(2)<<p.price<<setw(10)<<p.stock<<"\n";
    }

    void searchProducts() const {
        header("PRODUCT SEARCH"); string k=readLine("Search name/category: ");
        transform(k.begin(),k.end(),k.begin(),[](unsigned char c){return char(tolower(c));}); bool found=false;
        for(const auto& p:products){ string n=p.name,c=p.category;
            transform(n.begin(),n.end(),n.begin(),[](unsigned char x){return char(tolower(x));});
            transform(c.begin(),c.end(),c.begin(),[](unsigned char x){return char(tolower(x));});
            if(n.find(k)!=string::npos||c.find(k)!=string::npos){cout<<p.id<<" | "<<p.name<<" | "<<p.category<<" | Rs."<<p.price<<" | Stock: "<<p.stock<<"\n";found=true;}
        }
        if(!found) cout<<"No matching products found.\n"; pause();
    }

    void placeOrder(){
        header("NEW ORDER"); Order o; o.id=nextOrderId++; o.customerName=readLine("Customer name: ");
        if(o.customerName.empty()) o.customerName="Walk-in Customer";
        while(true){
            showMenu(); int id=readInt("\nProduct ID (0 to finish): ",0); if(id==0) break;
            Product* p=findProduct(id); if(!p||!p->available){cout<<"Product unavailable.\n";continue;}
            int q=readInt("Quantity: ",1); if(q>p->stock){cout<<"Only "<<p->stock<<" available.\n";continue;}
            auto it=find_if(o.items.begin(),o.items.end(),[id](const OrderItem& x){return x.productId==id;});
            if(it!=o.items.end()) it->quantity+=q; else o.items.push_back({p->id,p->name,q,p->price});
            cout<<"Added to cart.\n";
        }
        if(o.items.empty()){cout<<"Order cancelled: cart is empty.\n";pause();return;}
        for(const auto& x:o.items)o.subtotal+=x.quantity*x.unitPrice;
        if(o.subtotal>=1000)o.discount=o.subtotal*.10; else if(o.subtotal>=500)o.discount=o.subtotal*.05;
        o.total=o.subtotal-o.discount;
        cout<<"\nSubtotal: Rs."<<o.subtotal<<"\nDiscount: Rs."<<o.discount<<"\nTotal: Rs."<<o.total<<"\n";
        cout<<"1. Cash\n2. Card\n3. UPI\n"; int pm=readInt("Payment method: ",1); while(pm>3)pm=readInt("Choose 1-3: ",1);
        o.paymentMethod=(pm==1?"Cash":pm==2?"Card":"UPI");
        for(const auto& x:o.items) findProduct(x.productId)->stock-=x.quantity;
        orders.push_back(o); cout<<"\nOrder #"<<o.id<<" placed successfully.\n"; pause();
    }

    void viewOrders() const {
        header("ORDER HISTORY"); if(orders.empty()){cout<<"No orders found.\n";pause();return;}
        for(const auto& o:orders){cout<<"\nOrder #"<<o.id<<" | "<<o.customerName<<" | "<<o.status<<" | "<<o.paymentMethod<<"\n";
            for(const auto& x:o.items)cout<<"  - "<<x.productName<<" x"<<x.quantity<<" = Rs."<<x.quantity*x.unitPrice<<"\n";
            cout<<"  Total: Rs."<<o.total<<"\n";}
        pause();
    }

    void updateStatus(){
        header("UPDATE ORDER STATUS"); int id=readInt("Order ID: ",1);
        auto it=find_if(orders.begin(),orders.end(),[id](const Order& o){return o.id==id;});
        if(it==orders.end()){cout<<"Order not found.\n";pause();return;}
        cout<<"1. PLACED\n2. PREPARING\n3. READY\n4. COMPLETED\n5. CANCELLED\n";
        int c=readInt("New status: ",1); while(c>5)c=readInt("Choose 1-5: ",1);
        const vector<string> s={"PLACED","PREPARING","READY","COMPLETED","CANCELLED"}; it->status=s[c-1]; cout<<"Updated.\n"; pause();
    }

    void addProduct(){
        header("ADD PRODUCT"); int id=readInt("Product ID: ",1); if(findProduct(id)){cout<<"ID exists.\n";pause();return;}
        string n=readLine("Product name: "),c=readLine("Category: "); double price=readDouble("Price: "); int stock=readInt("Opening stock: ",0);
        products.push_back({id,n,c,price,stock,true}); cout<<"Product added.\n"; pause();
    }

    void restock(){
        header("RESTOCK"); int id=readInt("Product ID: ",1); Product* p=findProduct(id);
        if(!p){cout<<"Product not found.\n";pause();return;} int q=readInt("Quantity: ",1); p->stock+=q; cout<<"New stock: "<<p->stock<<"\n"; pause();
    }

    void inventoryReport() const {
        header("INVENTORY REPORT"); for(const auto& p:products)cout<<p.id<<" | "<<left<<setw(22)<<p.name<<" Stock: "<<setw(4)<<p.stock<<(p.stock<=5?" [LOW STOCK]":"")<<"\n"; pause();
    }

    void salesReport() const {
        header("SALES REPORT"); double revenue=0; map<string,int> sold;
        for(const auto& o:orders)if(o.status!="CANCELLED"){revenue+=o.total;for(const auto& x:o.items)sold[x.productName]+=x.quantity;}
        cout<<"Total orders: "<<orders.size()<<"\nRevenue: Rs."<<fixed<<setprecision(2)<<revenue<<"\n\nUnits sold:\n";
        for(const auto& [n,q]:sold)cout<<"  "<<left<<setw(25)<<n<<q<<"\n"; pause();
    }

    void admin(){
        if(readLine("Admin password: ")!="9014"){cout<<"Invalid password.\n";pause();return;}
        while(true){ header("ADMIN DASHBOARD"); cout<<"1. View products\n2. Add product\n3. Restock\n4. Inventory report\n5. Sales report\n6. View orders\n7. Update order status\n0. Back\n";
            int c=readInt("Choose: ",0); switch(c){case 1:header("PRODUCT MENU");showMenu();pause();break;case 2:addProduct();break;case 3:restock();break;case 4:inventoryReport();break;case 5:salesReport();break;case 6:viewOrders();break;case 7:updateStatus();break;case 0:return;default:cout<<"Invalid option.\n";}}
    }
public:
    CoffeeShop(){seedProducts();}
    void run(){
        while(true){header("COFFEE HOUSE MANAGEMENT SYSTEM - C++");cout<<"1. Browse menu\n2. Search products\n3. Place order\n4. View orders\n5. Admin dashboard\n0. Exit\n";
            int c=readInt("Choose: ",0); switch(c){case 1:header("MENU");showMenu();pause();break;case 2:searchProducts();break;case 3:placeOrder();break;case 4:viewOrders();break;case 5:admin();break;case 0:cout<<"Thank you for visiting Coffee House!\n";return;default:cout<<"Invalid option.\n";}}
    }
};

int main(){CoffeeShop app;app.run();return 0;}
