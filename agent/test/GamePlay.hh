#include "Db.hh"
#include "NddlUtils.hh"
/** Custom Include Files **/

namespace NDDL {

// GamePlay.nddl

  class StringData;
  typedef Id<StringData> StringDataId;
  typedef ObjectDomain StringDataDomain;
  
  class AgentTimeline;
  typedef Id<AgentTimeline> AgentTimelineId;
  typedef ObjectDomain AgentTimelineDomain;
  
  class AgentActions;
  typedef Id<AgentActions> AgentActionsId;
  typedef ObjectDomain AgentActionsDomain;
  
  class StateVariable;
  typedef Id<StateVariable> StateVariableId;
  typedef ObjectDomain StateVariableDomain;
  
  class CreatorTimeline;
  typedef Id<CreatorTimeline> CreatorTimelineId;
  typedef ObjectDomain CreatorTimelineDomain;
  
  class ReciverTimeline;
  typedef Id<ReciverTimeline> ReciverTimelineId;
  typedef ObjectDomain ReciverTimelineDomain;
  
  class EqReciverTimeline;
  typedef Id<EqReciverTimeline> EqReciverTimelineId;
  typedef ObjectDomain EqReciverTimelineDomain;
  
  class PlayerState;
  typedef Id<PlayerState> PlayerStateId;
  typedef ObjectDomain PlayerStateDomain;
  
  class Player;
  typedef Id<Player> PlayerId;
  typedef ObjectDomain PlayerDomain;
  
  class Umpire;
  typedef Id<Umpire> UmpireId;
  typedef ObjectDomain UmpireDomain;
  
  class NumberTimeline;
  typedef Id<NumberTimeline> NumberTimelineId;
  typedef ObjectDomain NumberTimelineDomain;
  
  class A_Timeline;
  typedef Id<A_Timeline> A_TimelineId;
  typedef ObjectDomain A_TimelineDomain;
  
  class B_Timeline;
  typedef Id<B_Timeline> B_TimelineId;
  typedef ObjectDomain B_TimelineDomain;
  
  class CyclicTimeline;
  typedef Id<CyclicTimeline> CyclicTimelineId;
  typedef ObjectDomain CyclicTimelineDomain;
  
  class SensorTimeline;
  typedef Id<SensorTimeline> SensorTimelineId;
  typedef ObjectDomain SensorTimelineDomain;
  
  class DerivedTimeline;
  typedef Id<DerivedTimeline> DerivedTimelineId;
  typedef ObjectDomain DerivedTimelineDomain;
  
  class Counter;
  typedef Id<Counter> CounterId;
  typedef ObjectDomain CounterDomain;
  
  class TestActions;
  typedef Id<TestActions> TestActionsId;
  typedef ObjectDomain TestActionsDomain;
  
  class ValueTimeline;
  typedef Id<ValueTimeline> ValueTimelineId;
  typedef ObjectDomain ValueTimelineDomain;
  
  class Valve;
  typedef Id<Valve> ValveId;
  typedef ObjectDomain ValveDomain;
  
  class ValveController;
  typedef Id<ValveController> ValveControllerId;
  typedef ObjectDomain ValveControllerDomain;
  
  class MotionSimulator;
  typedef Id<MotionSimulator> MotionSimulatorId;
  typedef ObjectDomain MotionSimulatorDomain;
  
  class HorizontalControl;
  typedef Id<HorizontalControl> HorizontalControlId;
  typedef ObjectDomain HorizontalControlDomain;
  
  class ScalabilityTestTimeline;
  typedef Id<ScalabilityTestTimeline> ScalabilityTestTimelineId;
  typedef ObjectDomain ScalabilityTestTimelineDomain;
  
  class OrienteeringTimeline;
  typedef Id<OrienteeringTimeline> OrienteeringTimelineId;
  typedef ObjectDomain OrienteeringTimelineDomain;
  
  class GraspObject;
  typedef Id<GraspObject> GraspObjectId;
  typedef ObjectDomain GraspObjectDomain;
  
  class Position;
  typedef Id<Position> PositionId;
  typedef ObjectDomain PositionDomain;
  
  
  
  // Core.nddl:8 TokenStates
  typedef SymbolDomain TokenStates;
  TokenStates TokenStatesBaseDomain();
  // SKIPPING DECLARATION FOR BUILT-IN CLASS Timeline
  
  
  // StringData.nddl:5 StringData
  class StringData : public Object {
  public:
    StringData(const PlanDatabaseId& planDatabase, const LabelStr& name);
    StringData(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    StringData(const ObjectId& parent, const LabelStr& name);
    StringData(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    ConstrainedVariableId data; // SchemaWriter::declareVariable
    
    // StringData.nddl:5 StringData
    virtual void constructor(const LabelStr& _data);
  };
  
  
  // TREX.nddl:9 Mode
  typedef SymbolDomain Mode;
  Mode ModeBaseDomain();
  
  // TREX.nddl:15 AgentTimeline
  class AgentTimeline : public Timeline {
  public:
    AgentTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    AgentTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    AgentTimeline(const ObjectId& parent, const LabelStr& name);
    AgentTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class undefined;
    typedef Id<undefined> undefinedId;
    class Terminated;
    typedef Id<Terminated> TerminatedId;
    ConstrainedVariableId mode; // SchemaWriter::declareVariable
    ConstrainedVariableId defaultPredicate; // SchemaWriter::declareVariable
    
    // TREX.nddl:23 undefined
    class undefined : public NddlToken {
    public:
      undefined(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      undefined(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(AgentTimeline::undefined, AgentTimeline.undefined);
      
    };
    
    // TREX.nddl:28 Terminated
    class Terminated : public NddlToken {
    public:
      Terminated(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Terminated(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(AgentTimeline::Terminated, AgentTimeline.Terminated);
      
    };
    
    // TREX.nddl:15 AgentTimeline
    virtual void constructor(const LabelStr& _mode);
    
    // TREX.nddl:15 AgentTimeline
    virtual void constructor(const LabelStr& _mode, const LabelStr& _defaultPredicate);
  };
  
  // TREX.nddl:45 AgentActions
  class AgentActions : public Object {
  public:
    AgentActions(const PlanDatabaseId& planDatabase, const LabelStr& name);
    AgentActions(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    AgentActions(const ObjectId& parent, const LabelStr& name);
    AgentActions(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    
    // TREX.nddl:45 AgentActions
    virtual void constructor();
  };
  
  // TREX.nddl:49 StateVariable
  class StateVariable : public AgentTimeline {
  public:
    StateVariable(const PlanDatabaseId& planDatabase, const LabelStr& name);
    StateVariable(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    StateVariable(const ObjectId& parent, const LabelStr& name);
    StateVariable(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // TREX.nddl:50 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(StateVariable::Holds, StateVariable.Holds);
      
    };
    
    // TREX.nddl:49 StateVariable
    virtual void constructor(const LabelStr& _mode);
    
    // TREX.nddl:49 StateVariable
    virtual void constructor(const LabelStr& _mode, const LabelStr& _defaultPredicate);
  };
  
  // dispatchlib.nddl:4 CreatorTimeline
  class CreatorTimeline : public AgentTimeline {
  public:
    CreatorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    CreatorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    CreatorTimeline(const ObjectId& parent, const LabelStr& name);
    CreatorTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Gamma;
    typedef Id<Gamma> GammaId;
    class Delta;
    typedef Id<Delta> DeltaId;
    
    // dispatchlib.nddl:5 Gamma
    class Gamma : public NddlToken {
    public:
      Gamma(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Gamma(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(CreatorTimeline::Gamma, CreatorTimeline.Gamma);
      
    };
    
    // dispatchlib.nddl:6 Delta
    class Delta : public NddlToken {
    public:
      Delta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Delta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(CreatorTimeline::Delta, CreatorTimeline.Delta);
      
    };
    
    // dispatchlib.nddl:4 CreatorTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // dispatchlib.nddl:18 ReciverTimeline
  class ReciverTimeline : public AgentTimeline {
  public:
    ReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    ReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    ReciverTimeline(const ObjectId& parent, const LabelStr& name);
    ReciverTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Alpha;
    typedef Id<Alpha> AlphaId;
    class Beta;
    typedef Id<Beta> BetaId;
    
    // dispatchlib.nddl:19 Alpha
    class Alpha : public NddlToken {
    public:
      Alpha(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Alpha(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ReciverTimeline::Alpha, ReciverTimeline.Alpha);
      
    };
    
    // dispatchlib.nddl:20 Beta
    class Beta : public NddlToken {
    public:
      Beta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Beta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ReciverTimeline::Beta, ReciverTimeline.Beta);
      
    };
    
    // dispatchlib.nddl:18 ReciverTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // dispatchlib.nddl:31 EqReciverTimeline
  class EqReciverTimeline : public AgentTimeline {
  public:
    EqReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    EqReciverTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    EqReciverTimeline(const ObjectId& parent, const LabelStr& name);
    EqReciverTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Alpha;
    typedef Id<Alpha> AlphaId;
    class Beta;
    typedef Id<Beta> BetaId;
    
    // dispatchlib.nddl:32 Alpha
    class Alpha : public NddlToken {
    public:
      Alpha(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Alpha(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(EqReciverTimeline::Alpha, EqReciverTimeline.Alpha);
      
    };
    
    // dispatchlib.nddl:33 Beta
    class Beta : public NddlToken {
    public:
      Beta(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Beta(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(EqReciverTimeline::Beta, EqReciverTimeline.Beta);
      
    };
    
    // dispatchlib.nddl:31 EqReciverTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  
  // GamePlay.nddl:4 Values
  typedef SymbolDomain Values;
  Values ValuesBaseDomain();
  
  // GamePlay.nddl:6 PlayerState
  class PlayerState : public AgentTimeline {
  public:
    PlayerState(const PlanDatabaseId& planDatabase, const LabelStr& name);
    PlayerState(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    PlayerState(const ObjectId& parent, const LabelStr& name);
    PlayerState(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Hides;
    typedef Id<Hides> HidesId;
    class Shows;
    typedef Id<Shows> ShowsId;
    
    // GamePlay.nddl:7 Hides
    class Hides : public NddlToken {
    public:
      Hides(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Hides(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(PlayerState::Hides, PlayerState.Hides);
      
    };
    
    // GamePlay.nddl:8 Shows
    class Shows : public NddlToken {
    public:
      Shows(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Shows(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(PlayerState::Shows, PlayerState.Shows);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:6 PlayerState
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:25 Player
  class Player : public AgentTimeline {
  public:
    Player(const PlanDatabaseId& planDatabase, const LabelStr& name);
    Player(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    Player(const ObjectId& parent, const LabelStr& name);
    Player(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Wait;
    typedef Id<Wait> WaitId;
    class One;
    typedef Id<One> OneId;
    class Two;
    typedef Id<Two> TwoId;
    class Three;
    typedef Id<Three> ThreeId;
    class Draw;
    typedef Id<Draw> DrawId;
    ConstrainedVariableId hand; // SchemaWriter::declareVariable
    
    // GamePlay.nddl:27 Wait
    class Wait : public NddlToken {
    public:
      Wait(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Wait(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Player::Wait, Player.Wait);
      
    };
    
    // GamePlay.nddl:28 One
    class One : public NddlToken {
    public:
      One(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      One(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Player::One, Player.One);
      
    };
    
    // GamePlay.nddl:29 Two
    class Two : public NddlToken {
    public:
      Two(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Two(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Player::Two, Player.Two);
      
    };
    
    // GamePlay.nddl:30 Three
    class Three : public NddlToken {
    public:
      Three(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Three(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Player::Three, Player.Three);
      
    };
    
    // GamePlay.nddl:31 Draw
    class Draw : public NddlToken {
    public:
      Draw(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Draw(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Player::Draw, Player.Draw);
      
    };
    
    // GamePlay.nddl:25 Player
    virtual void constructor(const LabelStr& _playerMode, const LabelStr& _stateMode);
  };
  
  // GamePlay.nddl:38 Umpire
  class Umpire : public AgentTimeline {
  public:
    Umpire(const PlanDatabaseId& planDatabase, const LabelStr& name);
    Umpire(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    Umpire(const ObjectId& parent, const LabelStr& name);
    Umpire(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class PlayGame;
    typedef Id<PlayGame> PlayGameId;
    class Wait;
    typedef Id<Wait> WaitId;
    ConstrainedVariableId player1; // SchemaWriter::declareVariable
    ConstrainedVariableId player2; // SchemaWriter::declareVariable
    
    // GamePlay.nddl:42 PlayGame
    class PlayGame : public NddlToken {
    public:
      PlayGame(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      PlayGame(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Umpire::PlayGame, Umpire.PlayGame);
      
    };
    
    // GamePlay.nddl:43 Wait
    class Wait : public NddlToken {
    public:
      Wait(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Wait(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Umpire::Wait, Umpire.Wait);
      
    };
    
    // GamePlay.nddl:38 Umpire
    virtual void constructor(const PlayerId& _player1, const PlayerId& _player2, const LabelStr& _mode);
  };
  
  // GamePlay.nddl:101 NumberTimeline
  class NumberTimeline : public AgentTimeline {
  public:
    NumberTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    NumberTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    NumberTimeline(const ObjectId& parent, const LabelStr& name);
    NumberTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class holds;
    typedef Id<holds> holdsId;
    
    // GamePlay.nddl:102 holds
    class holds : public NddlToken {
    public:
      holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(NumberTimeline::holds, NumberTimeline.holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:101 NumberTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:117 A_Timeline
  class A_Timeline : public AgentTimeline {
  public:
    A_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    A_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    A_Timeline(const ObjectId& parent, const LabelStr& name);
    A_Timeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class pred;
    typedef Id<pred> predId;
    
    // GamePlay.nddl:118 pred
    class pred : public NddlToken {
    public:
      pred(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      pred(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(A_Timeline::pred, A_Timeline.pred);
      
    };
    
    // GamePlay.nddl:117 A_Timeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:122 B_Timeline
  class B_Timeline : public AgentTimeline {
  public:
    B_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    B_Timeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    B_Timeline(const ObjectId& parent, const LabelStr& name);
    B_Timeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class pred1;
    typedef Id<pred1> pred1Id;
    class pred2;
    typedef Id<pred2> pred2Id;
    
    // GamePlay.nddl:123 pred1
    class pred1 : public NddlToken {
    public:
      pred1(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      pred1(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(B_Timeline::pred1, B_Timeline.pred1);
      
    };
    
    // GamePlay.nddl:124 pred2
    class pred2 : public NddlToken {
    public:
      pred2(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      pred2(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(B_Timeline::pred2, B_Timeline.pred2);
      
    };
    
    // GamePlay.nddl:122 B_Timeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:163 CyclicTimeline
  class CyclicTimeline : public AgentTimeline {
  public:
    CyclicTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    CyclicTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    CyclicTimeline(const ObjectId& parent, const LabelStr& name);
    CyclicTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class pred;
    typedef Id<pred> predId;
    
    // GamePlay.nddl:165 pred
    class pred : public NddlToken {
    public:
      pred(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      pred(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(CyclicTimeline::pred, CyclicTimeline.pred);
      
    };
    
    // GamePlay.nddl:163 CyclicTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:185 SensorTimeline
  class SensorTimeline : public AgentTimeline {
  public:
    SensorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    SensorTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    SensorTimeline(const ObjectId& parent, const LabelStr& name);
    SensorTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class holds;
    typedef Id<holds> holdsId;
    
    // GamePlay.nddl:186 holds
    class holds : public NddlToken {
    public:
      holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(SensorTimeline::holds, SensorTimeline.holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:185 SensorTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:193 DerivedTimeline
  class DerivedTimeline : public AgentTimeline {
  public:
    DerivedTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    DerivedTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    DerivedTimeline(const ObjectId& parent, const LabelStr& name);
    DerivedTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class holds;
    typedef Id<holds> holdsId;
    
    // GamePlay.nddl:194 holds
    class holds : public NddlToken {
    public:
      holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(DerivedTimeline::holds, DerivedTimeline.holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:193 DerivedTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:211 Counter
  class Counter : public AgentTimeline {
  public:
    Counter(const PlanDatabaseId& planDatabase, const LabelStr& name);
    Counter(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    Counter(const ObjectId& parent, const LabelStr& name);
    Counter(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // GamePlay.nddl:212 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Counter::Holds, Counter.Holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:211 Counter
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:221 TestActions
  class TestActions : public AgentActions {
  public:
    TestActions(const PlanDatabaseId& planDatabase, const LabelStr& name);
    TestActions(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    TestActions(const ObjectId& parent, const LabelStr& name);
    TestActions(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class increment;
    typedef Id<increment> incrementId;
    class multiply;
    typedef Id<multiply> multiplyId;
    class iterator;
    typedef Id<iterator> iteratorId;
    class doGrasp;
    typedef Id<doGrasp> doGraspId;
    
    // GamePlay.nddl:223 increment
    class increment : public NddlToken {
    public:
      increment(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      increment(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(TestActions::increment, TestActions.increment);
      
    };
    
    // GamePlay.nddl:225 multiply
    class multiply : public NddlToken {
    public:
      multiply(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      multiply(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(TestActions::multiply, TestActions.multiply);
      
      ConstrainedVariableId multiplier; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:227 iterator
    class iterator : public NddlToken {
    public:
      iterator(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      iterator(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(TestActions::iterator, TestActions.iterator);
      
      ConstrainedVariableId counter; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:229 doGrasp
    class doGrasp : public NddlToken {
    public:
      doGrasp(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      doGrasp(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(TestActions::doGrasp, TestActions.doGrasp);
      
      ConstrainedVariableId x; // SchemaWriter::declareVariable
      ConstrainedVariableId y; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:221 TestActions
    virtual void constructor();
  };
  
  // GamePlay.nddl:238 ValueTimeline
  class ValueTimeline : public AgentTimeline {
  public:
    ValueTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    ValueTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    ValueTimeline(const ObjectId& parent, const LabelStr& name);
    ValueTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // GamePlay.nddl:239 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ValueTimeline::Holds, ValueTimeline.Holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:238 ValueTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:302 Valve
  class Valve : public AgentTimeline {
  public:
    Valve(const PlanDatabaseId& planDatabase, const LabelStr& name);
    Valve(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    Valve(const ObjectId& parent, const LabelStr& name);
    Valve(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Open;
    typedef Id<Open> OpenId;
    class Closed;
    typedef Id<Closed> ClosedId;
    
    // GamePlay.nddl:303 Open
    class Open : public NddlToken {
    public:
      Open(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Open(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Valve::Open, Valve.Open);
      
    };
    
    // GamePlay.nddl:304 Closed
    class Closed : public NddlToken {
    public:
      Closed(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Closed(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Valve::Closed, Valve.Closed);
      
    };
    
    // GamePlay.nddl:302 Valve
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:318 ValveController
  class ValveController : public AgentTimeline {
  public:
    ValveController(const PlanDatabaseId& planDatabase, const LabelStr& name);
    ValveController(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    ValveController(const ObjectId& parent, const LabelStr& name);
    ValveController(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Inactive;
    typedef Id<Inactive> InactiveId;
    class InconsistentSlavesInExecution;
    typedef Id<InconsistentSlavesInExecution> InconsistentSlavesInExecutionId;
    class ValveStaysClosed;
    typedef Id<ValveStaysClosed> ValveStaysClosedId;
    
    // GamePlay.nddl:319 Inactive
    class Inactive : public NddlToken {
    public:
      Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ValveController::Inactive, ValveController.Inactive);
      
    };
    
    // GamePlay.nddl:320 InconsistentSlavesInExecution
    class InconsistentSlavesInExecution : public NddlToken {
    public:
      InconsistentSlavesInExecution(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      InconsistentSlavesInExecution(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ValveController::InconsistentSlavesInExecution, ValveController.InconsistentSlavesInExecution);
      
    };
    
    // GamePlay.nddl:321 ValveStaysClosed
    class ValveStaysClosed : public NddlToken {
    public:
      ValveStaysClosed(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      ValveStaysClosed(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ValveController::ValveStaysClosed, ValveController.ValveStaysClosed);
      
    };
    
    // GamePlay.nddl:318 ValveController
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:345 MotionSimulator
  class MotionSimulator : public AgentTimeline {
  public:
    MotionSimulator(const PlanDatabaseId& planDatabase, const LabelStr& name);
    MotionSimulator(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    MotionSimulator(const ObjectId& parent, const LabelStr& name);
    MotionSimulator(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // GamePlay.nddl:346 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(MotionSimulator::Holds, MotionSimulator.Holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:345 MotionSimulator
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:355 HorizontalControl
  class HorizontalControl : public AgentTimeline {
  public:
    HorizontalControl(const PlanDatabaseId& planDatabase, const LabelStr& name);
    HorizontalControl(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    HorizontalControl(const ObjectId& parent, const LabelStr& name);
    HorizontalControl(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // GamePlay.nddl:356 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(HorizontalControl::Holds, HorizontalControl.Holds);
      
      ConstrainedVariableId value; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:355 HorizontalControl
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:414 ScalabilityTestTimeline
  class ScalabilityTestTimeline : public AgentTimeline {
  public:
    ScalabilityTestTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    ScalabilityTestTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    ScalabilityTestTimeline(const ObjectId& parent, const LabelStr& name);
    ScalabilityTestTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    ConstrainedVariableId n; // SchemaWriter::declareVariable
    ConstrainedVariableId d; // SchemaWriter::declareVariable
    ConstrainedVariableId maxD; // SchemaWriter::declareVariable
    ConstrainedVariableId c; // SchemaWriter::declareVariable
    
    // GamePlay.nddl:419 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(ScalabilityTestTimeline::Holds, ScalabilityTestTimeline.Holds);
      
    };
    
    // GamePlay.nddl:414 ScalabilityTestTimeline
    virtual void constructor(int _n, int _d, int _maxD, int _c, const LabelStr& _mode);
    
    // GamePlay.nddl:414 ScalabilityTestTimeline
    virtual void constructor(int _n, int _d, int _c, const LabelStr& _mode);
  };
  
  
  // GamePlay.nddl:479 PRIORITY
  typedef IntervalIntDomain PRIORITY;
  PRIORITY PRIORITYBaseDomain();
  
  
  // GamePlay.nddl:480 OrienteerID
  typedef IntervalIntDomain OrienteerID;
  OrienteerID OrienteerIDBaseDomain();
  
  
  // GamePlay.nddl:481 PFLOAT
  typedef IntervalDomain PFLOAT;
  PFLOAT PFLOATBaseDomain();
  
  // GamePlay.nddl:483 OrienteeringTimeline
  class OrienteeringTimeline : public AgentTimeline {
  public:
    OrienteeringTimeline(const PlanDatabaseId& planDatabase, const LabelStr& name);
    OrienteeringTimeline(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    OrienteeringTimeline(const ObjectId& parent, const LabelStr& name);
    OrienteeringTimeline(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Inactive;
    typedef Id<Inactive> InactiveId;
    class Active;
    typedef Id<Active> ActiveId;
    
    // GamePlay.nddl:484 Inactive
    class Inactive : public NddlToken {
    public:
      Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(OrienteeringTimeline::Inactive, OrienteeringTimeline.Inactive);
      
      ConstrainedVariableId x; // SchemaWriter::declareVariable
      ConstrainedVariableId y; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:488 Active
    class Active : public NddlToken {
    public:
      Active(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Active(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(OrienteeringTimeline::Active, OrienteeringTimeline.Active);
      
      ConstrainedVariableId x; // SchemaWriter::declareVariable
      ConstrainedVariableId y; // SchemaWriter::declareVariable
      ConstrainedVariableId priority; // SchemaWriter::declareVariable
      ConstrainedVariableId fakeDuration; // SchemaWriter::declareVariable
      ConstrainedVariableId value; // SchemaWriter::declareVariable
      ConstrainedVariableId countertimeline; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:483 OrienteeringTimeline
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:531 GraspObject
  class GraspObject : public AgentTimeline {
  public:
    GraspObject(const PlanDatabaseId& planDatabase, const LabelStr& name);
    GraspObject(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    GraspObject(const ObjectId& parent, const LabelStr& name);
    GraspObject(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Inactive;
    typedef Id<Inactive> InactiveId;
    class Active;
    typedef Id<Active> ActiveId;
    ConstrainedVariableId param; // SchemaWriter::declareVariable
    
    // GamePlay.nddl:534 Inactive
    class Inactive : public NddlToken {
    public:
      Inactive(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Inactive(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(GraspObject::Inactive, GraspObject.Inactive);
      
    };
    
    // GamePlay.nddl:536 Active
    class Active : public NddlToken {
    public:
      Active(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Active(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(GraspObject::Active, GraspObject.Active);
      
      ConstrainedVariableId x; // SchemaWriter::declareVariable
      ConstrainedVariableId y; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:531 GraspObject
    virtual void constructor(const LabelStr& _mode);
  };
  
  // GamePlay.nddl:561 Position
  class Position : public AgentTimeline {
  public:
    Position(const PlanDatabaseId& planDatabase, const LabelStr& name);
    Position(const PlanDatabaseId& planDatabase, const LabelStr& type, const LabelStr& name);
    Position(const ObjectId& parent, const LabelStr& name);
    Position(const ObjectId& parent, const LabelStr& type, const LabelStr& name);
    void handleDefaults(bool autoClose = true); // default variable initialization
    
    class Holds;
    typedef Id<Holds> HoldsId;
    
    // GamePlay.nddl:562 Holds
    class Holds : public NddlToken {
    public:
      Holds(const PlanDatabaseId& planDatabase, const LabelStr& name, bool rejectable = false, bool isFact = false, bool close = false);
      Holds(const TokenId& parent, const LabelStr& name, const LabelStr& relation, bool close = false);
      void handleDefaults(bool autoClose = true); // default variable initialization
      DECLARE_TOKEN_FACTORY(Position::Holds, Position.Holds);
      
      ConstrainedVariableId x; // SchemaWriter::declareVariable
      ConstrainedVariableId y; // SchemaWriter::declareVariable
    };
    
    // GamePlay.nddl:561 Position
    virtual void constructor(const LabelStr& _mode);
  };
} // End NDDL namespace
