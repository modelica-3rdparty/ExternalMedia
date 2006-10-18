package Common 
  package InputChoices 
  "Type, constants and menu choices to define input variables, as temporary solution until enumerations are available" 
     extends Modelica.Icons.Enumeration;
  
    constant Integer dT=0 "(d,T) as inputs";
    constant Integer ph=1 "(p,h) as inputs";
    constant Integer ps=2 "(p,s) as inputs";
    constant Integer pT=3 "(p,T) as inputs";
  
    replaceable type InputChoice 
    extends Integer(min=0,max=3);
    annotation(Evaluate=true,choices(
      choice=ExternalMedia.Common.InputChoices.dT "(d,T) as inputs",
      choice=ExternalMedia.Common.InputChoices.ph "(p,h) as inputs",
      choice=ExternalMedia.Common.InputChoices.ps "(p,s) as inputs",
      choice=ExternalMedia.Common.InputChoices.pT "(p,T) as inputs"));
    end InputChoice;
  end InputChoices;
end Common;
