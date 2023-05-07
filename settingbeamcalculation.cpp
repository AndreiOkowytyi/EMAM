#include "settingbeamcalculation.h"

void SeсtionBeamCalculation::formСonfiguratioStandart(Ui::BeamCalculation* form){

    form->standart_profile_Ibeam_  ->close();
    form->standart_profile_channel_->close();
    form->standart_profile_corner_ ->close();
}

void SeсtionBeamCalculation::formСonfigurationB(Ui::BeamCalculation* form, bool position) {

    if(position) {
        form->section_scale_B_      ->show();
        form->section_size_B_       ->show();
        form->section_millimeters_B_->show();
    }else {
        form->section_scale_B_      ->close();
        form->section_size_B_       ->close();
        form->section_millimeters_B_->close();
    }
}

void SeсtionBeamCalculation::formСonfigurationH(Ui::BeamCalculation* form, bool position){

    if(position){
        form->section_scale_H_      ->show();
        form->section_size_H_       ->show();
        form->section_millimeters_H_->show();
    }else {
        form->section_scale_H_      ->close();
        form->section_size_H_       ->close();
        form->section_millimeters_H_->close();
    }
}

void SeсtionBeamCalculation::formConfigurationS(Ui::BeamCalculation* form, bool position){

    if(position){
        form->section_scale_S_      ->show();
        form->section_size_S_       ->show();
        form->section_millimeters_S_->show();
    }else {
        form->section_scale_S_      ->close();
        form->section_size_S_       ->close();
        form->section_millimeters_S_->close();
    }
}

void SeсtionBeamCalculation::formСonfigurationAxisXY(Ui::BeamCalculation* form, bool position){

    if(position){
       form->load_x_->show();
       form->load_y_->show();
    }else {
       form->load_x_->close();
       form->load_y_->close();
    }
}

void SectionBeamrRectangularPipe::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form);
    this->formConfigurationS      (form);
    this->formСonfigurationAxisXY (form);

    form->section_scale_B_->setText("B");
    form->section_scale_H_->setText("H");
    form->section_scale_S_->setText("S");
}

void SectionBeamSquarePipe::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form, false);

    form->section_scale_B_->setText("B");
    form->section_scale_H_->setText("S");
}

void SectionBeamRoundPipe::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form, false);

    form->section_scale_B_->setText("D");
    form->section_scale_H_->setText("S");
}

void SectionBeamCircle::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form, false);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form, false);

    form->section_scale_B_->setText("D");
}

void SectionBeamSquare::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form, false);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form, false);

    form->section_scale_B_->setText("B");
}

void SectionBeamLeaf::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form);
    this->formСonfigurationH      (form);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form);

    form->section_scale_B_->setText("B");
    form->section_scale_H_->setText("H");
}

void SectionBeamIBeam::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form, false);
    this->formСonfigurationH      (form, false);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form);

    form->standart_profile_Ibeam_->show();
}

void SectionBeamChannel::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form, false);
    this->formСonfigurationH      (form, false);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form);

    form->standart_profile_channel_->resize(form->standart_profile_Ibeam_->width(),
                                            form->standart_profile_Ibeam_->height());

    form->standart_profile_channel_->move  (form->standart_profile_Ibeam_->x(),

                                            form->standart_profile_Ibeam_->y());
    form->standart_profile_channel_->show();
}

void SectionBeamCorner::settingSectionForm(Ui::BeamCalculation* form){

    this->formСonfiguratioStandart(form);
    this->formСonfigurationB      (form, false);
    this->formСonfigurationH      (form, false);
    this->formConfigurationS      (form, false);
    this->formСonfigurationAxisXY (form, false);

    form->standart_profile_corner_->resize(form->standart_profile_Ibeam_->width(), form->standart_profile_Ibeam_->height());
    form->standart_profile_corner_->move  (form->standart_profile_Ibeam_->x(),     form->standart_profile_Ibeam_->y());

    form->standart_profile_corner_->show();
}

ConfigurationSection::ConfigurationSection(){

    this->options_.reserve(this->capacity_);

    options_.emplace(this->iterator(), std::make_unique<SectionBeamrRectangularPipe>());
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamSquarePipe));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamRoundPipe));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamCircle));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamSquare));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamLeaf));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamIBeam));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamChannel));
    options_.emplace(this->iterator(), std::unique_ptr<SeсtionBeamCalculation>(new SectionBeamCorner));
}

std::vector<std::unique_ptr<SeсtionBeamCalculation>>::iterator ConfigurationSection::iterator() {

    auto iterator_ = this->options_.begin();
    std::advance(iterator_, this->value_);
    if(this->value_ < this->capacity_) ++value_;

    return iterator_;
}

void ConfigurationSection::configImage(const int index,Ui::BeamCalculation *temp) {

    options_.at(index)->settingSectionForm(temp);
}


void SchemeBeamCalculation::formConfigurationL1(Ui::BeamCalculation* form, bool position) {

    if(position) {
        form->beam_length_l1_->show();
        form->field_L1_      ->show();
    }else {
        form->beam_length_l1_->close();
        form->field_L1_      ->close();
    }
}

void SchemeBeamCalculation::formConfigurationL2(Ui::BeamCalculation* form, bool position) {

    if(position) {
        form->beam_length_l2_->show();
        form->field_L2_      ->show();
    }else {
        form->beam_length_l2_->close();
        form->field_L2_      ->close();
    }
}

void SchemeTwoSupports::settingSchemeForm(Ui::BeamCalculation* form) {

    formConfigurationL1(form);
    formConfigurationL2(form);
    form->group_reaction_->show();
}

void SchemeTwoSupportsConsole::settingSchemeForm(Ui::BeamCalculation* form) {

    formConfigurationL1(form);
    formConfigurationL2(form);
    form->group_reaction_->show();
}

void SchemeConsole::settingSchemeForm(Ui::BeamCalculation* form) {

    formConfigurationL1(form);
    formConfigurationL2(form, false);
    form->group_reaction_->close();
}


ConfigurationScheme::ConfigurationScheme() {

    this->options_.reserve(this->capacity_);

    options_.emplace(this->iterator(), std::unique_ptr<SchemeBeamCalculation>(new SchemeTwoSupports));
    options_.emplace(this->iterator(), std::unique_ptr<SchemeBeamCalculation>(new SchemeTwoSupportsConsole));
    options_.emplace(this->iterator(), std::unique_ptr<SchemeBeamCalculation>(new SchemeConsole));
}

std::vector<std::unique_ptr<SchemeBeamCalculation>>::iterator ConfigurationScheme::iterator() {

    auto iterator_ = this->options_.begin();
    std::advance(iterator_, this->value_);
    if(this->value_ < this->capacity_) ++value_;

    return iterator_;
}

void ConfigurationScheme::configImage(const int index, Ui::BeamCalculation *temp) {

    this->options_.at(index)->settingSchemeForm(temp);
}
