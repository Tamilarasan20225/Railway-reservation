#include<bits/stdc++.h>
using namespace std;

                                            // Location class
class Location{
    public:
    vector<string>Locations;
     unordered_map<string,int>getIndex;
     Location(){
        Locations = {"Chennai","Arakkonam","m"};
        for(int i=0;i<Locations.size();i++){
            getIndex[Locations[i]] = i;
        }
     }
    
   
     void addLocation(string loc){
        Locations.push_back(loc);   // adding location to the list of locations
     }
     int getLocationIndex(string loc){
        return getIndex[loc];
     }
};

                            // users class
class Users{
    string name;
    string start_position;
    string end_position;
    double distance;
    double price;
    bool isBooked=false;
    int booking_id;
    int seat_no;
    char preferred_seat;

    public:
    Users(string name,string s,string e,char preferred_seat){
        this->name = name;
       this->start_position = s;
       this->end_position = e;
       Location* location=new Location();
       this->distance=location->getLocationIndex(this->end_position)-location->getLocationIndex(this->start_position);
       this->preferred_seat=preferred_seat;
    }

    string getName(){
        return name;
    }

    double getDistance(){
        return distance;
    }
    void setSeatNo(int n){
        seat_no = n;
    }
    void setBookingId(int id){
        booking_id=id;
    }
    char getPreferredSeat(){
        return preferred_seat;
    }
    string getStartPosition(){
        return start_position;
    }
    string getEndPosition(){
        return end_position;
    }
   
    void setPrice(double p){
        price=p;
    }
    double getPrice(){
        return price;
    }

    int getBookingId(){
        return booking_id;
    }
    void displayUserDetails(){
        cout<<"Booking Id: "<<booking_id<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Start Position: "<<start_position<<endl;
        cout<<"End Position: "<<end_position<<endl;
        cout<<"Distance Travelled: "<<distance<<endl;
        cout<<"Price paid: "<<price<<endl;
        cout<<"Seat No: "<<seat_no<<endl;
    }
};


class Train{
    int train_id;
    int earnings;
    int normal_fare;
    int premium_fare;
    string curr_position;
    vector<int>normal_available;
    vector<int>premium_available;
    unordered_map<int,Users*>booking_details;
    unordered_map<int,Users*>cancelled_details;
    Location* location;
    set<int>normal_seats;
    set<int>premium_seats;
    public:
    Train(){
        static int id=0;
        this->train_id=id++;
        this->earnings=0;
        this->normal_fare=10;
        this->premium_fare=20;
        this->curr_position="Chennai";
        location=new Location();
        normal_available.resize(location->Locations.size(),15);
        premium_available.resize(location->Locations.size(),15);
        for(int i=1;i<=15;i++) normal_seats.insert(i);
        for(int i=1;i<=15;i++) premium_seats.insert(i);
    }

    int AvailableTicketNormal(string loc){
        if(location->getIndex.find(loc)!=location->getIndex.end())
        return normal_available[location->getLocationIndex(loc)];
        cout<<"sorry the tickets are not available for this location";
        return 0;
    }

    int AvailableTicketPremium(string loc){
        if(location->getIndex.find(loc)!=location->getIndex.end())
        return premium_available[location->getLocationIndex(loc)];
        cout<<"sorry the tickets are not available for this location"<<endl;
        return 0;
    }
    

    void displayBookingDetails(){
        for(auto bookings:booking_details){
            Users *user = bookings.second;
            user->displayUserDetails();
        }
    }
    void displayCancellationDetails(){
        for(auto bookings:cancelled_details){
            Users *user = bookings.second;
            user->displayUserDetails();
        }
    }

    void displayTrainDetails(){
        cout<<"Train Id: "<<train_id<<endl;
        cout<<"Normal Fare: "<<normal_fare<<endl;
        cout<<"Premium Fare: "<<premium_fare<<endl;
       
        
    }
    void bookTheTrain(Users* user,int booking_id){
        string start_position=user->getStartPosition();
        string end_position=user->getEndPosition();
        char p=user->getPreferredSeat();
        double distance=user->getDistance();
        static int surge=10;
        if(p=='P') this->premium_fare+=surge;
        int fare=p=='P'?premium_fare:normal_fare;
        double price=fare*distance;
        user->setPrice(price);
        user->setSeatNo(*normal_seats.begin());
        normal_seats.erase(normal_seats.begin());
        user->setBookingId(booking_id);
        earnings+=price;
        for(int i=location->getLocationIndex(start_position);i<=location->getLocationIndex(end_position);i++){
            if(p=='P') normal_available[i]--;
            else premium_available[i]--;
        }
        booking_details[booking_id]=user;
        cout<<"BOOKED SUCCESSFULLY"<<endl;
    }
    void cancelTicket(int bookingId){
        if(booking_details.find(bookingId)!=booking_details.end()){
            Users *user = booking_details[bookingId];
           
            booking_details.erase(bookingId);
            cancelled_details[bookingId]=user;
            earnings-=user->getPrice();

            string start_position=user->getStartPosition();
            string end_position=user->getEndPosition();
            char p=user->getPreferredSeat();
            for(int i=location->getLocationIndex(start_position);i<=location->getLocationIndex(end_position);i++){
                if(p=='P') normal_available[i]++;
                else premium_available[i]++;
            }
        }
        else cout<<"Sorry No such User Exist"<<endl;
    }
    void displayEarnings(){
        cout<<"The Earnings of the train is: "<<earnings<<endl;
    }
        
    
};

class BookingSystem{
    unordered_map<string,Train*>bookingApp;

    public:

    void booking(string date,Users* user){
        static int id=0;
        
        if(bookingApp.find(date)==bookingApp.end()){
            Train *t=new Train();
            // cout<<"111";
            bookingApp[date]=t;
        }

        Train *train=bookingApp[date];
        string start_position=user->getStartPosition();
        string end_position=user->getEndPosition();
        char preferred_seat=user->getPreferredSeat();
        if(preferred_seat=='P'){
            if(train->AvailableTicketPremium(start_position)>0 and train->AvailableTicketPremium(end_position)>0){
                train->bookTheTrain(user,id++);
            }
            else cout<<"Sorry the tickets are not available"<<endl;
        }
        else if(preferred_seat=='N'){
            if(train->AvailableTicketNormal(start_position)>0 and train->AvailableTicketNormal(end_position)>0){
                train->bookTheTrain(user,id++);
            }
            else cout<<"Sorry the tickets are not available"<<endl;
        }

    }

    void cancelling(Users* user,string date){
        if(bookingApp.find(date)!=bookingApp.end()){
            Train* train=bookingApp[date];
            int bookingId=user->getBookingId();
            train->cancelTicket(bookingId);
        }
        else {
            cout<<"Sorry No such train exist in this date"<<endl;
        }
    }
    Train* getTrain(string date){
        if(bookingApp.find(date)!=bookingApp.end())
        return bookingApp[date];
        else{
            cout<<"Sorry No trains to fetch in this date";
            
        }
        return nullptr;
    }

    void TrainDetails(){
        for(auto& trains:bookingApp){
            Train* train=trains.second;
            cout<<"Date: "<<trains.first<<endl;
            train->displayTrainDetails();
        }
    }
};

int main(){

    Users user("R","Chennai","Arakkonam",'P');
    Users u("t","Chennai","Arakkonam",'P');
    Users u1("tuu","Chennai","Arakkonam",'P');

    BookingSystem bookingsystem;
    bookingsystem.booking("apr 30",&user);
    bookingsystem.booking("apr 30",&u);
    bookingsystem.booking("apr 30",&u1);

    Train * train=bookingsystem.getTrain("apr 30");
    train->displayBookingDetails();
    train->displayEarnings();


    cout<<endl;
    bookingsystem.cancelling(&user,"apr 30");
    cout<<endl;


    train->displayBookingDetails();
    cout<<endl;
    
    train->displayEarnings();
    
   

    return 0;
}