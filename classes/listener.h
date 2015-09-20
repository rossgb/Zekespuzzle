#include "entity.h"
#include "player.h"
#include "blueguy.h"
#include <iostream>

class ContactListener : public b2ContactListener
  {
    void BeginContact(b2Contact* contact) {
      // try {
      //   Entity* bodyAUserData = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData();
      //   Entity* bodyBUserData = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData();
      //   bodyAUserData ->handleCollision(bodyBUserData);
      //
      // } catch (int e) {
      //   std::cout << "not a body" << std::endl;
      // }
      Entity* A = NULL;
      Entity* B = NULL;
      if( (A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      (B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        A->handleCollision(B,1,contact->GetFixtureA(),contact->GetFixtureB());
        B->handleCollision(A,1,contact->GetFixtureB(),contact->GetFixtureA());
      }
      if( !(A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      (B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        B->handleGroundCollision(1,contact->GetFixtureB(),contact->GetFixtureA());
      }
      if( (A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      !(B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        A->handleGroundCollision(1,contact->GetFixtureA(),contact->GetFixtureB());
      }


      // contact->SetEnabled(false);

      // std::cout << contact->IsEnabled();


    }

    void EndContact(b2Contact* contact) {
      Entity* A = NULL;
      Entity* B = NULL;
      if( (A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      (B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        A->handleCollision(B,0,contact->GetFixtureA(),contact->GetFixtureB());
        B->handleCollision(A,0,contact->GetFixtureB(),contact->GetFixtureA());
      }

      if( !(A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      (B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        B->handleGroundCollision(0,contact->GetFixtureB(),contact->GetFixtureA());
      }
      if( (A = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData()) &&
      !(B = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData()) ) {
        A->handleGroundCollision(0,contact->GetFixtureA(),contact->GetFixtureB());
      }

    }
  };
