#include "ir.h"

void PrintMainfold(ir::Mainfold mf) {
    switch (mf.type) {
        case ir::Mainfold::Int: std::cout << mf.value.i << std::endl; break;
        case ir::Mainfold::Float: std::cout << mf.value.f << std::endl; break;
        case ir::Mainfold::String: std::cout << *mf.value.s << std::endl; break;
        case ir::Mainfold::Bool: std::cout << (mf.value.b ? "true" : "false") << std::endl; break;
        case ir::Mainfold::None: std::cout << "None\n"; break;
        case ir::Mainfold::Field: std::cout << "{field." << mf.value.mf << "}\n"; break;
        case ir::Mainfold::Object: std::cout << "{object." << mf.value.st << "}\n"; break;
        default: break;
    }
}

ir::Instruction::Instruction(Type instruction, std::vector<Instruction *> args) {
    this->instruction = instruction;
    this->args = args;
}

ir::Instruction::Instruction(Type instruction, std::string value, std::vector<Instruction *> args) {
    this->instruction = instruction;
    this->args = args;
    this->value.s = new std::string(value);
}

ir::Instruction::Instruction(Type instruction, int value, std::vector<Instruction *> args) {
    this->instruction = instruction;
    this->args = args;
    this->value.i = value;
}

ir::Instruction::~Instruction() {
    for (int i = 0; i < args.size(); i++) {
        delete args[i];
    }
}

void ir::Interpreter::Interpret(std::vector<Instruction *> instructions) {
    Mainfold mf = { Mainfold::None };

    SymbolTable* global = new SymbolTable();
    context.Add(global);
    context.SetCurrent(global);

    for (int i = 0; i < instructions.size(); i++) {
        mf = Interpret(instructions[i]); 
    }

    // PrintMainfold(mf);

    context.SetCurrent(nullptr);
    delete global;
}

#define ARG(i) Interpret(instruction->GetArg(i))

ir::Mainfold ir::Interpreter::Interpret(Instruction *instruction) {
    switch (instruction->GetInstruction()) {
        case IR: { 
            return { Mainfold::Scope, new Instruction(ir::Scope, instruction->GetArgs()) };
        }

        case If: {
            Mainfold ret = { Mainfold::None };
            
            if (ARG(0).value.b) {
                ret = Interpret(instruction->GetArg(1));
            }
            else if (instruction->GetArgs().size() > 2) {
                ret = Interpret(instruction->GetArg(2));
            }
            
            return ret;
        }

        case Call: {
            Mainfold name = ARG(0);

            if (name.type != Mainfold::String) {
                const std::vector<Instruction*> args = instruction->GetArgs();

                std::vector<Mainfold> argv;
                argv.resize(args.size() - 1);

                for (int i = 1; i < args.size(); i++) {
                    argv[i - 1] = Interpret(args[i]);
                }

                stack.push(argv);
                
                Mainfold* mf = ARG(0).value.mf;
                Mainfold ret = { Mainfold::None };

                if (mf->type == Mainfold::Scope) {
                    ret = Interpret(mf->value.ir);
                }

                stack.pop();

                return ret;
            }
            else {
                if (*name.value.s == "print") {
                    Mainfold arg = ARG(1);

                    if (arg.type == Mainfold::Field) {
                        PrintMainfold(*arg.value.mf);
                    }
                    else {
                        PrintMainfold(arg);
                    }
                }
            }

            return { Mainfold::None };
        }

        case Arg: { return stack.top()[instruction->value.i]; }

        case Scope: {
            SymbolTable* scope = new SymbolTable(context.GetCurrent());
            context.Add(scope);
            context.SetCurrent(scope);

            Mainfold mf = { Mainfold::None };

            for (int i = 0; i < instruction->GetArgs().size(); i++) {
                ir::Instruction *arg = instruction->GetArg(i);

                if (arg->GetInstruction() == ir::Type::Return) {
                    if (arg->GetArgs().size() > 0) {
                        mf = Interpret(instruction->GetArg(0));
                    }
                    break;
                }
                
                mf = Interpret(instruction->GetArg(i));
            }

            context.SetCurrent(scope->GetParent());
            delete scope;

            return mf;
        }

        // Variables
        case Decl: {
            context.GetCurrent()->Set(*instruction->value.s);
            return { Mainfold::None };
        }

        case Set: {
            Mainfold* mf = ARG(0).value.mf;
            Mainfold value = ARG(1);
            
            mf->type = value.type;
            mf->value = value.value;

            return { Mainfold::None };
        }

        case Var: {
            return { 
                Mainfold::Field,
                &context.GetCurrent()->Get(*instruction->value.s)
            };
        }

        // Objects
        case New: {
            Object* object = new Object(instruction->value.i);
            Mainfold mf = { Mainfold::Object, object };

            for (int i = 0; i < instruction->GetArgs().size(); i++) {
                object->fields.push_back(Interpret(instruction->GetArg(i)));
            }

            return mf;
        }

        case Field: {
            Object* object = ARG(0).value.mf->value.st;
            return { Mainfold::Field, &object->fields[instruction->value.i] };
        }

        // Operators
        case Add: { return { Mainfold::Int,  { ARG(0).value.i + ARG(1).value.i } }; }
        case Sub: { return { Mainfold::Int,  { ARG(0).value.i - ARG(1).value.i } }; }
        case Mul: { return { Mainfold::Int,  { ARG(0).value.i * ARG(1).value.i } }; }
        case Div: { return { Mainfold::Int,  { ARG(0).value.i / ARG(1).value.i } }; }
        case Mod: { return { Mainfold::Int,  { ARG(0).value.i % ARG(1).value.i } }; }
        case And: { return { Mainfold::Bool, { ARG(0).value.b && ARG(1).value.b } }; }
        case Or:  { return { Mainfold::Bool, { ARG(0).value.b || ARG(1).value.b } }; }
        case Xor: { return { Mainfold::Bool, { ARG(0).value.b ^ ARG(1).value.b } }; }
        case Not: { return { Mainfold::Bool, { !ARG(0).value.b } }; }
        case Shl: { return { Mainfold::Int,  { ARG(0).value.i << ARG(1).value.i } }; }
        case Shr: { return { Mainfold::Int,  { ARG(0).value.i >> ARG(1).value.i } }; }
        case Eq:  { return { Mainfold::Bool, { ARG(0).value.i == ARG(1).value.i } }; }
        case Neq: { return { Mainfold::Bool, { ARG(0).value.i != ARG(1).value.i } }; }
        case Lt:  { return { Mainfold::Bool, { ARG(0).value.i < ARG(1).value.i } }; }
        case Gt:  { return { Mainfold::Bool, { ARG(0).value.i > ARG(1).value.i } }; }
        case Leq: { return { Mainfold::Bool, { ARG(0).value.i <= ARG(1).value.i } }; }
        case Geq: { return { Mainfold::Bool, { ARG(0).value.i >= ARG(1).value.i } }; }
        
        // Literals
        case Int: return { Mainfold::Int, { instruction->value.i } };
        case Float: return { Mainfold::Float, { instruction->value.f } };
        case String: return { Mainfold::String, { instruction->value.s } };
        case Bool: return { Mainfold::Bool, { instruction->value.b } };
        default: break;
    }

    return { Mainfold::None };
}

inline ir::Mainfold& ir::SymbolTable::Get(std::string name) {
    if (symbols.find(name) == symbols.end()) {
        if (parent != nullptr) {
            return parent->Get(name);
        }
    }

    return symbols[name];
}
